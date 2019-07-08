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

class Vector3D
{
private:
    double X;
    double Y;
    double Z;

    double det_2(double a11, double a12, double a21, double a22) { return a11 * a22 - a21 * a12; }

public:
    Vector3D() : X(0), Y(0), Z(0) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}
    bool isNull() { return X == 0 && Y == 0 && Z == 0; }
    double getX() { return X; }
    double getY() { return Y; }
    double getZ() { return Z; }
    double scalar_multi(Vector3D v) { return X * v.X + Y * v.Y + Z * v.Z; }
    double mixed_multi(Vector3D v1, Vector3D v2)  { return scalar_multi(v1 * v2); }
    bool isCollinearity(Vector3D v)
    {
        return ((v.X == 0 || v.Y == 0) ? true : X / v.X == Y / v.Y) &&
                ((v.X == 0 || v.Z == 0) ? true : X / v.X == Z / v.Z) &&
                ((v.Y == 0 || v.Z == 0) ? true : Y / v.Y == Z / v.Z);
    }
    Vector3D operator=(Vector3D v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(Vector3D v) { return Vector3D(X+v.X, Y+v.Y, Z+v.Z); }
    Vector3D operator-(Vector3D v) { return Vector3D(X-v.X, Y-v.Y, Z-v.Z); }
    Vector3D operator*(Vector3D v) { return Vector3D(det_2(Y, Z, v.Y, v.Z), -det_2(X, Z, v.X, v.Z), det_2(X, Y, v.X, v.Y)); }
    Vector3D operator*(double k) { return Vector3D(X*k, Y*k, Z*k); }
    bool operator==(Vector3D v) { return X == v.X && Y == v.Y && Z == v.Z; }
    friend ostream & operator<<(ostream &, Vector3D);
};

ostream & operator<<(ostream &out, Vector3D v) { out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")"; return out; }

// -------------------------------------------------------------------

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    Vector3D getPoint(double ratio) { return start + getDirection() * ratio; } // parametric view
    double getRatio(Vector3D point) { return (point.getX() - start.getX()) / getDirection().getX(); }
    bool isInsideSegment(Vector3D point) { double ratio = getRatio(point); return getRatio(start) <= ratio && ratio <= getRatio(end); }
public:
    Segment3D() : start(0,0,0), end(1,1,1) {}
    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {
        if (getDirection().isNull()) throw Exception("beginning and end of line must be different points");
    }
    bool isCollinearity(Segment3D line) // belong to a line of segment
    {
        Vector3D point1 = line.start - start;
        Vector3D point2 = line.end - start;
        Vector3D dir = getDirection();
        return point1.isCollinearity(dir) && point2.isCollinearity(dir);
    }
    Vector3D getDirection() { return end - start; }
    bool isParallel(Segment3D line) { return getDirection().isCollinearity(line.getDirection()); }
    bool isCoplanarity(Segment3D line) // belong to a common surface
    {
        Vector3D vec = line.start - start;
        return vec.mixed_multi(getDirection(), line.getDirection()) == 0;
    }
    friend ostream& operator<<(ostream &, Segment3D);
    friend int Intersect(Segment3D s1, Segment3D s2, Vector3D &cross);
};

ostream& operator<<(ostream &out, Segment3D s) { out << s.start << " -> " << s.end; return out; }

int Intersect(Segment3D s1, Segment3D s2, Vector3D &cross)
{
    if (s1.isCollinearity(s2)) return -1; // lines belong to a common line
    if (s1.isParallel(s2)) return -2; // lines is parallel
    if (!s1.isCoplanarity(s2)) return 2; // not belong to a common surface

    Vector3D s1_v = s1.getDirection();
    Vector3D s2_v = s2.getDirection();
    Vector3D dif = s1.start - s2.start;
    double ratio = (s1_v.getY() * dif.getX() - s1_v.getX() * dif.getY()) / (s1_v.getY() * s2_v.getX() - s1_v.getX() * s2_v.getY());
    cross = s2.getPoint(ratio);

    if (!s1.isInsideSegment(cross) || !s2.isInsideSegment(cross)) return 1;

    return 0;
}

void printResultIntersect(Segment3D s1, Segment3D s2)
{
    Vector3D point;
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;

    int res = Intersect(s1, s2, point);

    if (res == 0) { cout << "point of intersection is " << point << endl; }
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
    Segment3D s1, s2;

    s1 = {{1, 1, 0}, {2, 2, 0}};
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
    cout << endl;

    return 0;
}
