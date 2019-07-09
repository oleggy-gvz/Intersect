#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Exception: public exception
{
private:
    string m_error;
public:
    Exception(string error) : m_error(error)
    {}
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

bool equal_real(double a, double b)
{
    double eps = 0.0000001;
    double abs = (a - b) < 0 ? b - a : a - b;
    return abs < eps ? true : false;
}

class Vector3D
{
private:
    double X;
    double Y;
    double Z;

public:
    Vector3D() : X(0), Y(0), Z(0)
    {}

    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z)
    {}

    double getX() const
    {
        return X;
    }

    double getY() const
    {
        return Y;
    }

    double getZ() const
    {
        return Z;
    }

    double scalar_multi(const Vector3D &v)
    {
        return X * v.X + Y * v.Y + Z * v.Z;
    }

    double mixed_multi(const Vector3D &v1, const Vector3D &v2)
    {
        return scalar_multi(v1 * v2);
    }

    bool isCollinearity(const Vector3D &v)
    {
        bool is_x = !equal_real(v.X, 0);
        bool is_y = !equal_real(v.Y, 0);
        bool is_z = !equal_real(v.Z, 0);
        bool res;

        if (!is_x && is_y && is_z)
            res = equal_real(Y / v.Y, Z / v.Z);
        else if (is_x && !is_y && is_z)
            res = equal_real(X / v.X, Z / v.Z);
        else if (is_x && is_y && !is_z)
            res = equal_real(X / v.X, Y / v.Y);
        else
            res = equal_real(X / v.X, Y / v.Y) && equal_real(Y / v.Y, Z / v.Z);

        return res;
    }

    Vector3D operator=(const Vector3D &v)
    {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
        return *this;
    }

    Vector3D operator+(const Vector3D &v) const
    {
        return Vector3D(X+v.X, Y+v.Y, Z+v.Z);
    }

    Vector3D operator-(const Vector3D &v) const
    {
        return Vector3D(X-v.X, Y-v.Y, Z-v.Z);
    }

    Vector3D operator*(const Vector3D &v) const
    {
        return Vector3D(Y * v.Z - v.Y * Z, -X * v.Z + v.X * Z, X * v.Y - v.X * Y);
    }

    Vector3D operator*(double k) const
    {
        return Vector3D(X * k, Y * k, Z * k);
    }

    bool operator==(const Vector3D &v)
    {
        return equal_real(X, v.X) && equal_real(Y, v.Y) && equal_real(Z, v.Z);
    }

    friend ostream & operator<<(ostream &, const Vector3D &);
};

ostream & operator<<(ostream &out, const Vector3D &v)
{
    out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return out;
}

// -------------------------------------------------------------------

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    struct Ratio
    {
        double x = 0, y = 0, z = 0;
        bool is_x = true, is_y = true, is_z = true;
    };

    Vector3D getPoint(Ratio ratio) const
    {
        Vector3D res;

        if (!ratio.is_y || !ratio.is_z)
            res = start + getDirection() * ratio.x;
        else if (!ratio.is_x || !ratio.is_z)
            res = start + getDirection() * ratio.y;
        else if (!ratio.is_x || !ratio.is_y)
            res = start + getDirection() * ratio.z;
        else // any
            res = start + getDirection() * ratio.x;

        return res;
    }

    bool isInsideSegment(const Vector3D &point) const
    {
        Ratio ratio = getRatio(point);
        bool res;

        if (!ratio.is_y || !ratio.is_z)
            res = getRatio(start).x <= ratio.x && ratio.x <= getRatio(end).x;
        else if (!ratio.is_x || !ratio.is_z)
            res = getRatio(start).y <= ratio.y && ratio.y <= getRatio(end).y;
        else if (!ratio.is_x || !ratio.is_y)
            res = getRatio(start).z <= ratio.z && ratio.z <= getRatio(end).z;
        else // any
            res = getRatio(start).x <= ratio.x && ratio.x <= getRatio(end).x;

        return res;
    }

    Ratio getRatio(const Vector3D &point) const
    {
        Vector3D dir = getDirection();
        Ratio ratio;

        ratio.is_x = !equal_real(dir.getX(), 0);
        ratio.x = ratio.is_x ? (point.getX() - start.getX()) / dir.getX() : 0;
        ratio.is_y = !equal_real(dir.getY(), 0);
        ratio.y = ratio.is_y ? (point.getY() - start.getY()) / dir.getY() : 0;
        ratio.is_z = !equal_real(dir.getZ(), 0);
        ratio.z = ratio.is_z ? (point.getZ() - start.getZ()) / dir.getZ() : 0;

        return ratio;
    }

    bool isRatioIntersect(double a1, double a2, double b1, double b2, double dif1, double dif2, double &ratio)
    {
        //ratio = (s1_v.getY() * dif.getX() - s1_v.getX() * dif.getY()) / (s1_v.getY() * s2_v.getX() - s1_v.getX() * s2_v.getY());
        if (equal_real(a2 * b1, a1 * b2))
            return false;
        else
            ratio = (a2 * dif1 - a1 * dif2) / (a2 * b1 - a1 * b2);
        return true;
    }

    Ratio getRatioIntersect(const Segment3D &line)
    {
        Vector3D s1_v = getDirection();
        Vector3D s2_v = line.getDirection();
        Vector3D dif = start - line.start;

        Ratio ratio;
        ratio.is_x = isRatioIntersect(s1_v.getX(), s1_v.getY(), s2_v.getX(), s2_v.getY(), dif.getX(), dif.getY(), ratio.x);
        ratio.is_y = isRatioIntersect(s1_v.getX(), s1_v.getZ(), s2_v.getX(), s2_v.getZ(), dif.getX(), dif.getZ(), ratio.y);
        ratio.is_z = isRatioIntersect(s1_v.getY(), s1_v.getZ(), s2_v.getY(), s2_v.getZ(), dif.getY(), dif.getZ(), ratio.z);

        return ratio;
    }

public:
    Segment3D() : start(0,0,0), end(1,1,1)
    {}

    Segment3D(const Vector3D &_start, const Vector3D &_end) : start(_start), end(_end)
    {
        if (getDirection() == Vector3D(0, 0 ,0)) throw Exception("beginning and end of line must be different points");
    }

    bool isCollinearity(const Segment3D &line) // belong to a line of segment
    {
        Vector3D vec1 = line.start - start;
        Vector3D vec2 = line.end - start;
        Vector3D dir = getDirection();
        bool tmp1 = vec1.isCollinearity(dir);
        bool tmp2 = vec2.isCollinearity(dir);
        return vec1.isCollinearity(dir) &&
                vec2.isCollinearity(dir);
    }

    Vector3D getDirection() const
    {
        return end - start;
    }

    bool isParallel(const Segment3D &line)
    {
        return getDirection().isCollinearity(line.getDirection());
    }

    bool isCoplanarity(const Segment3D &line) // belong to a common surface
    {
        Vector3D vec = line.start - start;
        return vec.mixed_multi(getDirection(), line.getDirection()) == 0;
    }

    int Intersect(const Segment3D &line, Vector3D &cross)
    {
        if (isCollinearity(line)) return -1; // lines belong to a common line
        if (isParallel(line)) return -2; // lines is parallel
        if (!isCoplanarity(line)) return 2; // not belong to a common surface

        Ratio ratio_line = getRatioIntersect(line);
        cross = line.getPoint(ratio_line);

        if (!isInsideSegment(cross) || !line.isInsideSegment(cross))
            return 1;

        return 0;
    }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s)
{
    out << s.start << " -> " << s.end;
    return out;
}

void printResultIntersect(const Segment3D &s1, const Segment3D &s2)
{
    Vector3D point;
    Segment3D _s1(s1);
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;

    int res = _s1.Intersect(s2, point);

    if (res == 0)
    {
        cout << "point of intersection is " << point << endl;
    }
    else
    {
        switch(res)
        {
        case 1:
            cout << "no intersect: point of intersection " << point << " is outside the boundaries of the segments" << endl;
            break;
        case 2:
            cout << "no intersect: lines is not belong to a common surface" << endl;
            break;
        case -1:
            cout << "no solutions: lines belong to a common line" << endl;
            break;
        case -2:
            cout << "no solutions: lines is parallel" << endl;
            break;
        }
    }
}

// -------------------------------------------------------------------

int main()
{
    Segment3D s1, s2;

    /*s1 = {{1, 1, 0}, {2, 2, 0}};
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

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{0, 3, 0}, {1, 2, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    // segments are set as starting point and vector
    Vector3D A, B, vec_a, vec_b;
    A = {3, -3, 2}; vec_a = {-1, 1, 2}; s1 = {A, A + vec_a};
    B = {-1, 4, -26}; vec_b = {3, -4, 6}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;*/

    s1 = {{2, 0, 0}, {1, 0, 0}};
    s2 = {{0, 2, 0}, {0, 1, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    return 0;
}
