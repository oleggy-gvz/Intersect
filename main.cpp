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

public:
    Vector3D() : X(0), Y(0), Z(0) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}

    Vector3D operator=(const Vector3D &v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(const Vector3D &v) { Vector3D tmp(X+v.X, Y+v.Y, Z+v.Z); return tmp; }
    Vector3D operator-(const Vector3D &v) { Vector3D tmp(X-v.X, Y-v.Y, Z-v.Z); return tmp; }

    double getX() { return X; }
    double getY() { return Y; }
    double getZ() { return Z; }
    bool isNull() { return X == 0 && Y == 0 && Z == 0; }

    //void setPoint(double _X, double _Y, double _Z) { X = _X; Y = _Y; Z = _Z; }
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

    Vector3D direction;
    struct coeff_equation  { double a, b, c, d; }; // equation coefficients: a*x + b*y + c*z + d = 0
    coeff_equation line; // line equation
    coeff_equation surf1, surf2; // two surface equation, alternative line equation as intersection of two surface

    double det_M2x2(double a11, double a12, double a21, double a22) { return a11 * a22 - a21 * a12; }
    //double det_M2x2(double a[2][2]) { return a[0][0] * a[1][1] - a[1][0] * a[0][1]; }
    double det_M2x2(double a[4]) { return a[0] * a[3] - a[2] * a[1]; }

    coeff_equation calculateSurfaceRatios(Vector3D first, Vector3D second, Vector3D third) // calculation of equation surface by three points
    {
        // TODO: проверка на коллинеарность
        coeff_equation s;
        Vector3D vec1 = second - first;
        Vector3D vec2 = third - first;

        s.a =  det_M2x2(vec1.getY(), vec2.getY(), vec1.getZ(), vec2.getZ()); // for X
        s.b = -det_M2x2(vec1.getX(), vec2.getX(), vec1.getZ(), vec2.getZ()); // for Y
        s.c =  det_M2x2(vec1.getX(), vec2.getX(), vec1.getY(), vec2.getY()); // for Z
        s.d = -first.getX() * s.a + first.getY() * (-s.b) - first.getZ() * s.c;
        return s;
    }

    void calculateRatios()
    {
        direction = end - start;
        if (direction.isNull()) throw Exception("points do not have to be equal");

        surf1 = calculateSurfaceRatios(start, end, {0, -1, 2});
        //surf2 = calculateSurfaceRatios(start, end, Vector3D());
    }

public:
    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {
        calculateRatios();
    }

    friend ostream& operator<<(ostream &, const Segment3D &);
    friend Vector3D Intersect(Segment3D s1, Segment3D s2);
};

ostream& operator<<(ostream &out, const Segment3D &s)
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
    Segment3D s2({1, -2, 0}, {2, 0, -1});
    cout << "s2 = " << s2 << endl;

    return 0;
}
