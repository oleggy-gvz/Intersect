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
    //Vector3D(Vector3D &v) : X(v.getX()), Y(v.getY()), Z(v.getZ()) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}

    Vector3D operator=(const Vector3D &v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(const Vector3D &v) { Vector3D tmp(X+v.X, Y+v.Y, Z+v.Z); return tmp; }
    Vector3D operator-(const Vector3D &v) { Vector3D tmp(X-v.X, Y-v.Y, Z-v.Z); return tmp; }

    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }
    bool isNull() { return X == 0 && Y == 0 && Z == 0; }

    friend ostream & operator<<(ostream &, const Vector3D &);
};

ostream & operator<<(ostream &out, const Vector3D &v)
{
    out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return out;
}

// -------------------------------------------------------------------

/*class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    Vector3D direction;
    struct coeff_equation  { double A, B, C, D; }; // equation coefficients: A*x + B*y + C*z + D = 0
    coeff_equation surf1, surf2; // two surface equation, alternative line equation as intersection of two surface

    double det_M2x2(double a11, double a12, double a21, double a22) { return a11 * a22 - a21 * a12; }
    //double det_M2x2(double a[2][2]) { return a[0][0] * a[1][1] - a[1][0] * a[0][1]; }
    double det_M2x2(double a[4]) { return a[0] * a[3] - a[2] * a[1]; }

    bool isCollinearity(Vector3D p1, Vector3D p2, Vector3D p_check)
    {
        Vector3D dir = p2 - p1;
        Vector3D vec = p_check - p1;
        return (vec.getX() / dir.getX()) == (vec.getY() / dir.getY()) &&
               (vec.getX() / dir.getX()) == (vec.getZ() / dir.getZ());
    }

    coeff_equation calculateSurfaceRatios(Vector3D first, Vector3D second, Vector3D third) // calculation of equation surface by three points
    {
        if (isCollinearity(first, second, third)) throw Exception("all three points are collinear");
        Vector3D vec1 = second - first;
        Vector3D vec2 = third - first;

        coeff_equation s;
        s.A =  det_M2x2(vec1.getY(), vec2.getY(), vec1.getZ(), vec2.getZ()); // for X
        s.B = -det_M2x2(vec1.getX(), vec2.getX(), vec1.getZ(), vec2.getZ()); // for Y
        s.C =  det_M2x2(vec1.getX(), vec2.getX(), vec1.getY(), vec2.getY()); // for Z
        s.D = -first.getX() * s.A + first.getY() * (-s.B) - first.getZ() * s.C;
        return s;
    }

    void calculateRatios()
    {
        direction = end - start;
        if (direction.isNull()) throw Exception("points do not have to be equal");

        Vector3D third_surf_1(direction.getZ(), direction.getX(), direction.getY());
        surf1 = calculateSurfaceRatios(start, end, third_surf_1);

        Vector3D third_surf_2(direction.getY(), direction.getZ(), direction.getX());
        surf2 = calculateSurfaceRatios(start, end, third_surf_2);
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
}*/

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;
    Vector3D direction;
public:
    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {
        direction = end - start;
        if (direction.isNull()) throw Exception("beginning and end of line must be different points");
    }
    bool isCollinearity(Vector3D point) // check of collinearity of point on the line p1-p2
    {
        Vector3D vec = point - start;
        return (vec.getX() / direction.getX()) == (vec.getY() / direction.getY()) &&
               (vec.getX() / direction.getX()) == (vec.getZ() / direction.getZ());
    }
    Vector3D getStart() const { return start; }
    Vector3D getEnd() const { return end; }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s)
{
    out << s.start << " -> " << s.end;
    return out;
}

class Matrix3D
{
private:
    double det_M2x2(double a11, double a12, double a21, double a22) { return a11 * a22 - a21 * a12; }
    //double det_M2x2(double a[2][2]) { return a[0][0] * a[1][1] - a[1][0] * a[0][1]; }
    //double det_M2x2(double a[4]) { return a[0] * a[3] - a[2] * a[1]; }
public:

};

int Intersect(Segment3D s1, Segment3D s2, Vector3D *cross)
{
    if (s1.isCollinearity(s2.getStart()) && s1.isCollinearity(s2.getEnd())) return 1;
    // TODO: не лежат на одной плоскости
    // TODO: параллельны

    return 0;
}

// -------------------------------------------------------------------

int main()
{
    /*Segment3D s1({0, 0, 0}, {2, 2, 2});
    Segment3D s2({3, 3, 3}, {5, 5, 5});*/

    Segment3D s1({1, 1, 0}, {3, 2, 0});
    Segment3D s2({1, 2, 0}, {3, 1, 0});

    Vector3D point;
    cout << "segment1 = " << s1 << endl;
    cout << "segment2 = " << s2 << endl;

    int res;
    res = Intersect(s1, s2, &point);

    switch(res)
    {
    case 0:
        cout << "cross point = " << point << endl;
        break;

    case 1:
        cout << "no solutions: lines is collinear, lie on one straight line" << endl;
        break;

    case 2:
        cout << "no solutions: " << endl;
        break;

    case 3:
        cout << "no solutions: " << endl;
        break;
    }

    return 0;
}
