#ifndef MATRIX_3D_H
#define MATRIX_3D_H

#include "Vector3D.h"

class Matrix3D
{
protected:
    Vector3D vectors[4]; // 3*4 matrix is represented as 4 rows of Vector3D
    unsigned int rows;
    unsigned int cols;

public:
    Matrix3D(const Matrix3D &m) { *this = m; }
    Matrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { setMatrix3D(v1, v2, v3); }
    Matrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &v4) { setMatrix3D(v1, v2, v3, v4); }
    void setMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { rows = 3; cols = 3; vectors[0] = v1; vectors[1] = v2; vectors[2] = v3; }
    void setMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &v4) { rows = 4; cols = 3; vectors[0] = v1; vectors[1] = v2; vectors[2] = v3; vectors[3] = v4; }
    unsigned int getRows() { return rows; }
    unsigned int getColums() { return cols; }
    void setRow(unsigned int row, const Vector3D &v)
    {
        if (row >= rows) throw Exception(Exception::MATRIX3D_OUT_RANGE);
        vectors[row] = v;
    }
    Vector3D getRow(unsigned int row)
    {
        if (row >= rows) throw Exception(Exception::MATRIX3D_OUT_RANGE);
        return vectors[row];
    }
    double getIndex(unsigned int row, unsigned int col)
    {
        if (row >= rows || col >= cols) throw Exception(Exception::MATRIX3D_OUT_RANGE);
        return vectors[row].getParam(col);
    }
    double det()
    {
        if (rows != 3 || cols != 3) throw Exception(Exception::MATRIX3D_SIZE_FOR_DET);
        return vectors[0].mixed_multi(vectors[1], vectors[2]);
    }
    Matrix3D& operator=(const Matrix3D &m)
    {
        if (this == &m) return *this;
        rows = m.rows; cols = m.cols;
        for (unsigned int i = 0; i < rows; i++) vectors[i] = m.vectors[i];
        return *this;
    }
    friend ostream& operator<<(ostream &, const Matrix3D &);
};

ostream& operator<<(ostream &out, const Matrix3D &m)
{
     for (unsigned int row = 0; row < m.rows; row++)
     {
        out << "|";
        for (unsigned int col = 0; col < m.cols; col++)
        {
            out.width(3);
            out << m.vectors[row].getParam(col) << (col == m.rows-1 ? "" : " ");
        }
        out << "|" << (row == m.rows-1 ? "" : "\n");
    }
    return out;
}

#endif // MATRIX_3D_H
