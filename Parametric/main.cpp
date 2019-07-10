#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Exception: public exception
{
private:
    string m_error;
public:
    Exception(string error) : m_error(error) {}
    const char* what() const noexcept { return m_error.c_str(); }
};

bool equal_real(double a, double b)
{
    double eps = 0.0000001;
    double abs = a < b ? b - a : a - b;
    return abs < eps ? true : false;
}

class Vector3D
{
private:
    double X;
    double Y;
    double Z;

public:
    Vector3D() : X(0), Y(0), Z(0) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}
    Vector3D(const Vector3D &p) : X(p.getX()), Y(p.getY()), Z(p.getZ()) {}

    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }

    double scalar_multi(const Vector3D &v) { return X * v.X + Y * v.Y + Z * v.Z; }
    double mixed_multi(const Vector3D &v1, const Vector3D &v2) { return scalar_multi(v1 * v2); }

    bool isCollinearity(const Vector3D &v)
    {
        bool is_x = !equal_real(v.X, 0);
        bool is_y = !equal_real(v.Y, 0);
        bool is_z = !equal_real(v.Z, 0);
        bool res;
        if (!is_x && is_y && is_z)      res = equal_real(Y / v.Y, Z / v.Z); // axis x is null
        else if (is_x && !is_y && is_z) res = equal_real(X / v.X, Z / v.Z); // axis y is null
        else if (is_x && is_y && !is_z) res = equal_real(X / v.X, Y / v.Y); // axis z is null
        else /* any other cases */      res = equal_real(X / v.X, Y / v.Y) && equal_real(Y / v.Y, Z / v.Z);
        return res;
    }

    Vector3D operator=(const Vector3D &p) { X = p.X; Y = p.Y; Z = p.Z; return *this; }
    Vector3D operator+(const Vector3D &p) const { return Vector3D(X+p.X, Y+p.Y, Z+p.Z); }
    Vector3D operator-(const Vector3D &p) const { return Vector3D(X-p.X, Y-p.Y, Z-p.Z); }
    Vector3D operator*(double k) const { return Vector3D(X * k, Y * k, Z * k); }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(Y * v.Z - v.Y * Z, -X * v.Z + v.X * Z, X * v.Y - v.X * Y); }
    bool operator==(const Vector3D &p) { return equal_real(X, p.X) && equal_real(Y, p.Y) && equal_real(Z, p.Z); }

    friend ostream & operator<<(ostream &, const Vector3D &);
};

ostream & operator<<(ostream &out, const Vector3D &p) { out << "(" << p.X << ", " << p.Y << ", " << p.Z << ")"; return out; }

// -------------------------------------------------------------------

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    struct Ratio { double x = 0, y = 0, z = 0; bool isX = true, isY = true, isZ = true; }; // bool - no axis

    Vector3D getVectorFromRario(Ratio ratio) const
    {
        Vector3D res;
        Vector3D dir = getDirection();
        if (ratio.isX)      res = start + dir * ratio.x;
        else if (ratio.isY) res = start + dir * ratio.y;
        else if (ratio.isZ) res = start + dir * ratio.z;
        else                res = start + dir * ratio.x; // any axis
        return res;
    }

    Ratio getRatio(const Vector3D &point) const
    {
        Vector3D diff = point - start;
        Vector3D dir = getDirection();
        Ratio ratio;

        ratio.isX = !equal_real(dir.getX(), 0);
        ratio.x = ratio.isX ? diff.getX() / dir.getX() : -1;
        ratio.isY = !equal_real(dir.getY(), 0);
        ratio.y = ratio.isY ? diff.getY() / dir.getY() : -1;
        ratio.isZ = !equal_real(dir.getZ(), 0);
        ratio.z = ratio.isZ ? diff.getZ() / dir.getZ() : -1;
        return ratio;
    }

    bool isInsideSegment(const Vector3D &point) const
    {
        Ratio ratio = getRatio(point);
        Ratio ratio_start = getRatio(start);
        Ratio ratio_end = getRatio(end);
        bool res;
        if (ratio.isX)      res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x;
        else if (ratio.isY) res = ratio_start.y <= ratio.y && ratio.y <= ratio_end.y;
        else if (ratio.isZ) res = ratio_start.z <= ratio.z && ratio.z <= ratio_end.z;
        else                res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x; // any axis
        return res;
    }

    bool ratioIntersect(double a1, double a2, double b1, double b2, double dif1, double dif2, double &ratio) const
    {
        bool res = false; ratio = -1; // mark empty ratio
        if (!equal_real(a2 * b1, a1 * b2)) { res = true; ratio = (a2 * dif1 - a1 * dif2) / (a2 * b1 - a1 * b2); } // check divide by zero
        return res;
    }

    Ratio getRatioIntersect(const Segment3D &line) const
    {
        Vector3D s1_v = getDirection();
        Vector3D s2_v = line.getDirection();
        Vector3D dif = start - line.start;
        Ratio ratio;
        ratio.isX = ratioIntersect(s1_v.getY(), s1_v.getZ(), s2_v.getY(), s2_v.getZ(), dif.getY(), dif.getZ(), ratio.x);
        ratio.isY = ratioIntersect(s1_v.getX(), s1_v.getZ(), s2_v.getX(), s2_v.getZ(), dif.getX(), dif.getZ(), ratio.y);
        ratio.isZ = ratioIntersect(s1_v.getX(), s1_v.getY(), s2_v.getX(), s2_v.getY(), dif.getX(), dif.getY(), ratio.z);
        return ratio;
    }

public:
    Segment3D(const Vector3D &_start, const Vector3D &_end) : start(_start), end(_end)
    {
        if (getDirection() == Vector3D(0, 0 ,0)) throw Exception("start and end points of the segment must be different");
    }
    bool isCollinearity(const Segment3D &line) const // belong to a line of segment
    {
        Vector3D vec1 = line.start - start;
        Vector3D vec2 = line.end - start;
        return vec1.isCollinearity(getDirection()) && vec2.isCollinearity(getDirection());
    }
    bool isParallel(const Segment3D &line) const { return getDirection().isCollinearity(line.getDirection()); }
    Vector3D getDirection() const { return end - start; }
    bool isCoplanarity(const Segment3D &line) const // belong to a common surface
    {
        Vector3D vec = line.start - start;
        return equal_real(vec.mixed_multi(getDirection(), line.getDirection()), 0);
    }
    int Intersect(const Segment3D &line, Vector3D &cross) const
    {
        if (isCollinearity(line)) return -1; // lines belong to a common line
        if (isParallel(line)) return -2; // lines is parallel
        if (!isCoplanarity(line)) return 2; // not belong to a common surface
        Ratio ratio = getRatioIntersect(line);
        cross = line.getVectorFromRario(ratio);
        if (!isInsideSegment(cross) || !line.isInsideSegment(cross)) return 1;
        return 0;
    }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s) { out << s.start << " -> " << s.end; return out; }

void printResultIntersect(const Segment3D &s1, const Segment3D &s2)
{
    Vector3D point;
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;
    int res = s1.Intersect(s2, point);
    if (res == 0) cout << "point of intersection is " << point << endl;
    else
    {
        switch(res)
        {
        case 1: cout << "no intersect: point of intersection " << point << " is outside the boundaries of the segments" << endl; break;
        case 2: cout << "no intersect: lines is not belong to a common surface" << endl; break;
        case -1: cout << "no solutions: lines belong to a common line" << endl; break;
        case -2: cout << "no solutions: lines is parallel" << endl; break;
        }
    }
}

// -------------------------------------------------------------------

int main()
{
    Segment3D s1 = {{1, 1, 0}, {2, 2, 0}},
            s2 = {{-3, -3, 0}, {-4, -4, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{2, 1, 0}, {3, 2, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{1, 2, 0}, {2, 1, 0.1}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{1, 2, 0}, {2, 1, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{0, 0.5, 2}, {0, 0.5, 1}};
    s2 = {{0, 2, 0.5}, {0, 1, 0.5}};
    printResultIntersect(s1, s2);
    cout << endl;

    Vector3D A = {3, -3, 2}; Vector3D vec_a = {-1, 1, 2}; s1 = {A, A + vec_a};
    Vector3D B = {-1, 4, -26}; Vector3D vec_b = {3, -4, 6}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;

    return 0;
}
