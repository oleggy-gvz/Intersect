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
    //Vector3D(Vector3D &v) : X(v.getX()), Y(v.getY()), Z(v.getZ()) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}

    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }
    bool isNull() { return X == 0 && Y == 0 && Z == 0; }

    Vector3D vector_multiplication(const Vector3D &v)  { Vector3D tmp(det_2(Y, Z, v.Y, v.Z), -det_2(X, Z, v.X, v.Z), det_2(X, Y, v.X, v.Y)); return tmp; }
    double scalar_multiplication(const Vector3D &v) { return X * v.getX() + Y * v.getY() + Z * v.getZ(); }
    double mixed_multiplication(Vector3D v1, Vector3D v2)  { Vector3D tmp = v1.vector_multiplication(v2); return scalar_multiplication(tmp); }
    bool isCollinearity(Vector3D v) { return (X / v.getX()) == (Y / v.getY()) && (X / v.getX()) == (Z / v.getZ()); }

    Vector3D operator=(const Vector3D &v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(const Vector3D &v) { Vector3D tmp(X+v.X, Y+v.Y, Z+v.Z); return tmp; }
    Vector3D operator-(const Vector3D &v) { Vector3D tmp(X-v.X, Y-v.Y, Z-v.Z); return tmp; }
    Vector3D operator*(const Vector3D &v) { return vector_multiplication(v); }

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

public:
    Vector3D getStart() const { return start; }
    Vector3D getEnd() const { return end; }
    Vector3D getDirection() const { return direction; }

    Segment3D(Vector3D _start, Vector3D _end) : start(_start), end(_end)
    {
        direction = end - start;
        if (direction.isNull()) throw Exception("beginning and end of line must be different points");
    }
    /*
    bool isCollinearity(Vector3D point) // belong to a segment
    {
        Vector3D vec = point - start;
        return (vec.getX() / direction.getX()) == (vec.getY() / direction.getY()) &&
               (vec.getX() / direction.getX()) == (vec.getZ() / direction.getZ());
    }
    bool isCollinearity(Segment3D line) // belong to a common line
    {
        return isCollinearity(line.getStart()) &&
                isCollinearity(line.getEnd());
    }*/

    bool isCollinearity(Segment3D line) // belong to a current segment
    {

    }

    bool isParallel(Segment3D line)
    {
        return direction.isCollinearity(line.getDirection());
    }

    bool isCoplanarity(Segment3D line) // belong to a common surface
    {
        Vector3D vec = line.getStart() - start;
        return vec.mixed_multiplication(direction, line.direction) == 0;
    }

    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s)
{
    out << s.start << " -> " << s.end;
    return out;
}

int Intersect(Segment3D s1, Segment3D s2, Vector3D *cross)
{
    if (s1.isCollinearity(s2)) return 1;
    if (s1.isParallel(s2)) return 2;
    if (!s1.isCoplanarity(s2)) return 3;

    // TODO: точка пересечения лежит за границами сегментов
    //if () return 4;

    return 0;
}

void printResultIntersect(Segment3D s1, Segment3D s2)
{
    Vector3D point;
    cout << "segment1 = " << s1 << endl;
    cout << "segment2 = " << s2 << endl;

    int res = Intersect(s1, s2, &point);

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
    /*Segment3D s1({0, 0, 0}, {2, 2, 2});
    Segment3D s2({3, 3, 3}, {5, 5, 5});*/

    Segment3D s1({1, 1, 0}, {3, 2, 0});
    Segment3D s2({1, 2, 0}, {3, 1, 0});

    printResultIntersect(s1, s2);

    return 0;
}
