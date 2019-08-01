#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <iostream>
#include <math.h>
#include "Exeption.h"
#include "NumComp.h"

using namespace std;

class Vector3D
{
private:
    double X;
    double Y;
    double Z;

    void set(const Vector3D &v) { if (this == &v) return; X = v.X; Y = v.Y; Z = v.Z; }
public:
    Vector3D() : X(0), Y(0), Z(0) {}
    Vector3D(double x, double y, double z) {X = x; Y = y; Z = z; }
    Vector3D(const Vector3D &v) { set(v); }
    double getX() { return X; }
    double getY() { return Y; }
    double getZ() { return Z; }
    double getLenght() { return sqrt(X*X + Y*Y + Z*Z); }
    double getParam(unsigned int index) const
    {
        double val;
        switch(index)
        {
        case 0: val = X; break;
        case 1: val = Y; break;
        case 2: val = Z; break;
        default: throw Exception(Exception::VECTOR3D_OUT_RANGE);
        }
        return val;
    }
    void setParam(unsigned int index, double val)
    {
        switch(index)
        {
        case 0: X = val; break;
        case 1: Y = val; break;
        case 2: Z = val; break;
        default: throw Exception(Exception::VECTOR3D_OUT_RANGE);
        }
    }
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
        else res = equal_real(X / v.X, Y / v.Y) && equal_real(Y / v.Y, Z / v.Z); // any other cases
        return res;
    }
    const Vector3D& operator=(const Vector3D &v) { set(v); return *this; }
    Vector3D operator*(double k) { return Vector3D(X * k, Y * k, Z * k); }
    Vector3D operator/(double k) { return Vector3D(X / k, Y / k, Z / k); }

    //Vector3D operator+(const Vector3D &v) const { return Vector3D(X+v.X, Y+v.Y, Z+v.Z); }
    friend const Vector3D operator+(const Vector3D &left, const Vector3D &rigth);
    //Vector3D operator-(const Vector3D &v) const { return Vector3D(X-v.X, Y-v.Y, Z-v.Z); }
    friend const Vector3D operator-(const Vector3D &left, const Vector3D &rigth);
    //Vector3D operator*(const Vector3D &v) const { return Vector3D(Y * v.Z - v.Y * Z, -X * v.Z + v.X * Z, X * v.Y - v.X * Y); }
    friend const Vector3D operator*(const Vector3D &left, const Vector3D &rigth);

    bool operator==(const Vector3D &v) { return equal_real(X, v.X) && equal_real(Y, v.Y) && equal_real(Z, v.Z); }
    friend ostream& operator<<(ostream &, const Vector3D &);
};

const Vector3D operator+(const Vector3D &left, const Vector3D &rigth) { return Vector3D(left.X+rigth.X, left.Y+rigth.Y, left.Z+rigth.Z); }

const Vector3D operator-(const Vector3D &left, const Vector3D &rigth) { return Vector3D(left.X-rigth.X, left.Y-rigth.Y, left.Z-rigth.Z); }

const Vector3D operator*(const Vector3D &left, const Vector3D &rigth)
{
    return Vector3D(left.Y * rigth.Z - rigth.Y * left.Z, -left.X * rigth.Z + rigth.X * left.Z, left.X * rigth.Y - rigth.X * left.Y);
}

ostream& operator<<(ostream &out, const Vector3D &v)
{
    out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return out;
}

#endif // VECTOR_3D_H
