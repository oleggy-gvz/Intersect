#include <iostream>

using namespace std;

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

    Vector3D operator=(const Vector3D &v)
    {
        X = v.X; Y = v.Y; Z = v.Z;
        return *this;
    }

    Vector3D operator+(const Vector3D &v)
    {
        Vector3D tmp(X+v.X, Y+v.Y, Z+v.Z);
        return tmp;
    }

    double getX() { return X; }
    double getY() { return Y; }
    double getZ() { return Z; }

    void setX(double _X) { X = _X; }
    void setY(double _Y) { Y = _Y; }
    void setZ(double _Z) { Z = _Z; }

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

    // line equation as intersection of two surface
    double a1, b1, c1, d1; // a1*x + b1*y + c1*z + d1 = 0 - surface №1
    double a2, b2, c2, d2; // a2*x + b2*y + c2*z + d2 = 0 - surface №2

public:
    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {}

    friend ostream & operator<<(ostream &, const Segment3D &);
    friend Vector3D Intersect(Segment3D s1, Segment3D s2);
};

ostream & operator<<(ostream &out, const Segment3D &s)
{
    out << s.start << " -> " << s.end;
    return out;
}

Vector3D Intersect(Segment3D s1, Segment3D s2)
{


}

// -------------------------------------------------------------------

int main()
{
    Vector3D v(1, 2, 0);
    Vector3D a1(v), a2;
    a2 = v;
    a2 = a1 + a2;

    cout << "a2 = " << a2 << endl;

    Segment3D s1(a1, a2);
    cout << "s1 = " << s1 << endl;

    Vector3D b1(2, 2, 0), b2(1, 4, 0);
    Segment3D s2(b1, b2);
    cout << "s2 = " << s2 << endl;

    return 0;
}
