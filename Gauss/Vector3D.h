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
    unsigned int size;

public:
    Vector3D() : X(0), Y(0), Z(0), size(3) {}
    Vector3D(double x, double y, double z) : size(3) { setParam(0, x); setParam(1, y); setParam(2, z); }
    Vector3D(const Vector3D &v) { *this = v; }
    double getX() { return X; }
    double getY() { return Y; }
    double getZ() { return Z; }
    double getSize() const { return size; }
    double getLenght() { return sqrt(X*X + Y*Y + Z*Z); }
    double getParam(unsigned int index) const
    {
        if (index >= size) throw Exception(Exception::VECTOR3D_OUT_RANGE);
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
        if (index >= size) throw Exception(Exception::VECTOR3D_OUT_RANGE);
        switch(index)
        {
        case 0: X = equal_real(val, 0) ? 0 : val; break;
        case 1: Y = equal_real(val, 0) ? 0 : val; break;
        case 2: Z = equal_real(val, 0) ? 0 : val; break;
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
    Vector3D& operator=(const Vector3D &v)
    {
        if (this == &v) return *this;
        size = v.size;
        for (unsigned int i = 0; i < size; i++) setParam(i, v.getParam(i));
        return *this;
    }
    Vector3D operator+(const Vector3D &v) const { return Vector3D(X+v.X, Y+v.Y, Z+v.Z); }
    Vector3D operator-(const Vector3D &v) const { return Vector3D(X-v.X, Y-v.Y, Z-v.Z); }
    Vector3D operator*(double k) const { return Vector3D(X * k, Y * k, Z * k); }
    Vector3D operator/(double k) const { return Vector3D(X / k, Y / k, Z / k); }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(Y * v.Z - v.Y * Z, -X * v.Z + v.X * Z, X * v.Y - v.X * Y); }
    Vector3D operator+=(const Vector3D &v) { Vector3D tmp = *this; *this = *this + v; return tmp; }
    bool operator==(const Vector3D &v) const { return equal_real(X, v.X) && equal_real(Y, v.Y) && equal_real(Z, v.Z); }
    friend ostream& operator<<(ostream &, const Vector3D &);
};

ostream& operator<<(ostream &out, const Vector3D &v)
{
    out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return out;
}

#endif // VECTOR_3D_H
