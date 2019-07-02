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

    void setPoint(double _X, double _Y, double _Z) { X = _X; Y = _Y; Z = _Z; }

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
    // surface equation: a*x + b*y + c*z + d = 0
    struct surface_ratios
    {
         double a, b, c, d;
    };
    surface_ratios surf[1]; // coefficients of two surfaces
    double det_M2x2(double a11, double a12, double a21, double a22) { return a11 * a22 - a21 * a12; }

    surface_ratios calculateRatios(Vector3D first, Vector3D second, Vector3D third)
    {
        surface_ratios s;
        s.a = det_M2x2(second.getY() - first.getY(), third.getY() - start.getY(),
                      second.getZ() - first.getZ(), third.getZ() - start.getZ());
        s.b = -det_M2x2(second.getX() - first.getX(), third.getX() - start.getX(),
                      second.getZ() - first.getZ(), third.getZ() - start.getZ());
        s.c = det_M2x2(second.getX() - first.getX(), third.getX() - start.getX(),
                      second.getY() - first.getY(), third.getY() - start.getY());
        s.d = -start.getX() * s.a + start.getY() * (-s.b) - start.getZ() * s.c;
        return s;
    }

    void calculateRatios()
    {
        surf[0] = calculateRatios(start, end, {0, -1, 2});
        //surf[1] = calculateRatios(start, end, Vector3D());
    }

public:
    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {
        calculateRatios();
    }

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

    //Segment3D s1(a1, a2);
    //cout << "s1 = " << s1 << endl;

    //Vector3D b1(2, 2, 0), b2(1, 4, 0);
    Vector3D b1(1, -2, 0), b2(2, 0, -1);
    Segment3D s2(b1, b2);
    cout << "s2 = " << s2 << endl;

    return 0;
}
