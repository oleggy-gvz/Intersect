#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <iostream>
#include "Exeption.h"

using namespace std;

bool equal_real(double a, double b)
{
    double eps = 0.0000001;
    double abs = a < b ? b - a : a - b;
    return abs < eps ? true : false;
}

class Vector3D
{
private:
    double X;
    double Y;
    double Z;

    unsigned int size;

public:
    Vector3D() : X(0), Y(0), Z(0), size(3) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z), size(3) {}
    Vector3D(const Vector3D &v) { *this = v; }
    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }
    double getSize() const { return size; }
    double getParam(unsigned int index) const
    {
        if (index >= size) throw Exception(Exception::VECTOR3D_OUT_RANGE);
        else
        {
            switch(index)
            {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            }
        }
    }
    void setParam(unsigned int index, double val)
    {
        if (index >= size) throw Exception(Exception::VECTOR3D_OUT_RANGE);
        else
        {
            switch(index)
            {
            case 0: X = val; break;
            case 1: Y = val; break;
            case 2: Z = val; break;
            }
        }
    }
    double scalar_multi(const Vector3D &v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    double mixed_multi(const Vector3D &v1, const Vector3D &v2) const { return scalar_multi(v1 * v2); }
    bool isCollinearity(const Vector3D &v) const
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
    Vector3D& operator=(const Vector3D &v) { if (this == &v) return *this; X = v.X; Y = v.Y; Z = v.Z; return *this; }

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
