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

class Segment3D;

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

    double scalar_multi(Vector3D v) { return X * v.X + Y * v.Y + Z * v.Z; }
    double mixed_multi(Vector3D v1, Vector3D v2)  { return scalar_multi(v1 * v2); }
    bool isCollinearity(Vector3D v)
    {
        bool eqv_XY = (v.X == 0 || v.Y == 0) ? true : X / v.X == Y / v.Y;
        bool eqv_XZ = (v.X == 0 || v.Z == 0) ? true : X / v.X == Z / v.Z;
        bool eqv_YZ = (v.Y == 0 || v.Z == 0) ? true : Y / v.Y == Z / v.Z;
        return eqv_XY && eqv_XZ && eqv_YZ;
    }
    Vector3D operator=(Vector3D v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(Vector3D v) { return Vector3D(X+v.X, Y+v.Y, Z+v.Z); }
    Vector3D operator-(Vector3D v) { return Vector3D(X-v.X, Y-v.Y, Z-v.Z); }
    Vector3D operator*(Vector3D v) { return Vector3D(det_2(Y, Z, v.Y, v.Z), -det_2(X, Z, v.X, v.Z), det_2(X, Y, v.X, v.Y)); }
    Vector3D operator*(double k) { return Vector3D(X*k, Y*k, Z*k); }
    bool operator==(Vector3D v) { return X == v.X && Y == v.Y && Z == v.Z; }

    friend ostream & operator<<(ostream &, Vector3D);
    friend int Intersect(Segment3D, Segment3D, Vector3D&);
};

ostream & operator<<(ostream &out, Vector3D v)
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

    Vector3D getPoint(double t) { return start + getDirection() * t; }
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
        return point1.isCollinearity(getDirection()) && point2.isCollinearity(getDirection());
    }
    Vector3D getDirection() { return end - start; }

    bool isParallel(Segment3D line) { return getDirection().isCollinearity(line.getDirection()); }
    bool isCoplanarity(Segment3D line) // belong to a common surface
    {
        Vector3D vec = line.start - start;
        return vec.mixed_multi(getDirection(), line.getDirection()) == 0;
    }

    friend ostream& operator<<(ostream &, Segment3D);
    friend int Intersect(Segment3D, Segment3D, Vector3D&);
};

ostream& operator<<(ostream &out, Segment3D s)
{
    out << s.start << " -> " << s.end;
    return out;
}

int Intersect(Segment3D s1, Segment3D s2, Vector3D &cross)
{
    if (s1.isCollinearity(s2)) return 1;
    if (s1.isParallel(s2)) return 2;
    if (!s1.isCoplanarity(s2)) return 3;

    // TODO: поиск точки пересечения
    Vector3D num = s2.start - s1.start;
    Vector3D denom = s1.getDirection() - s2.getDirection();

    double t_x = num.X / denom.X;
    double t_y = num.Y / denom.Y;
    double t_z = num.Z / denom.Z;
    double t;
    if (s1.getPoint(t_x) == s2.getPoint(t_x)) t = t_x;
    else if (s1.getPoint(t_y) == s2.getPoint(t_y)) t = t_y;
    else if (s1.getPoint(t_z) == s2.getPoint(t_z)) t = t_z;
    else throw Exception("unknown error in the process of calculating the intersection point");

    cross = s1.getPoint(t);
    // TODO: точка пересечения лежит за границами сегментов
    //if () return 4;

    return 0;
}

void printResultIntersect(Segment3D s1, Segment3D s2)
{
    Vector3D point;
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;

    int res = Intersect(s1, s2, point);

    switch(res)
    {
    case 0:
        cout << "point of intersection is " << point << endl;
        break;

    case 1:
        cout << "no solutions: belong to a common line" << endl;
        break;

    case 2:
        cout << "no solutions: lines is parallel" << endl;
        break;

    case 3:
        cout << "no intersect: lines is not belong to a common surface" << endl;
        break;

    case 4:
        cout << "no intersect: point of intersection is outside the boundaries of the segments" << endl;
        break;
    }
}

// -------------------------------------------------------------------

int main()
{
    Segment3D s1, s2;
    printResultIntersect(s1, s2);
    cout << endl;

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

    /*s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{1, 2, 0}, {2, 1, 0}};
    printResultIntersect(s1, s2);
    cout << endl;*/

    /*Vector3D p1, p2;
    p1 = {3, -3, 2}; p2 = p1 + Vector3D(-1, 1, 2);
    s1 = {p1, p2};
    p1 = {-1, 4, -26}; p2 = p1 + Vector3D(3, -4, 6);
    s2 = {p1, p2};

    printResultIntersect(s1, s2);*/
    cout << endl;

    return 0;
}
