#ifndef EXT_MATRIX_3D_H
#define EXT_MATRIX_3D_H

#include "Vector3D.h"

class ExtMatrix3D
{
private:
    double matrix[4][4] = {{0}};
    unsigned int rows;
    unsigned int cols;

    double det_2(int i1, int i2,int j1, int j2) { return matrix[i1][j1] * matrix[i2][j2] - matrix[i2][j1] * matrix[i1][j2]; }
    double det_3(int i1, int i2, int i3, int j1, int j2, int j3)
    {
        return matrix[i1][j1] * det_2(i2, i3, j2, j3) - matrix[i1][j2] * det_2(i2, i3, j1, j3) + matrix[i1][j3] * det_2(i2, i3, j1, j2);
    }
    double det_4(int i1, int i2, int i3, int i4, int j1, int j2, int j3, int j4)
    {
        return matrix[i1][j1] * det_3(i2, i3, i4, j2, j3, j4) - matrix[i1][j2] * det_3(i2, i3, i4, j1, j3, j4) +
                matrix[i1][j3] * det_3(i2, i3, i4, j1, j2, j4) - matrix[i1][j4] * det_3(i2, i3, i4, j1, j2, j3);
    }

public:
    ExtMatrix3D(const ExtMatrix3D &m) { *this = m; }
    ExtMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { setMatrix(v1, v2, v3); }
    ExtMatrix3D(const Vector3D v[4], double b[4]) { setMatrix(v, b); }
    void setMatrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
    {
        rows = 3; cols = 3;
        for (unsigned int col = 0; col < cols; col++){ matrix[0][col] = v1.getParam(col); matrix[1][col] = v2.getParam(col); matrix[2][col] = v3.getParam(col); }
    }
    void setMatrix(const Vector3D v[4], double result[4])
    {
        rows = 4; cols = 4;
        for (unsigned int row = 0; row < rows; row++)
            for (unsigned int col = 0; col < v->getSize(); col++) matrix[row][col] = v[row].getParam(col);
        for (unsigned int row = 0; row < rows; row++) matrix[row][cols-1] = result[row];
    }
    unsigned int getRows() const { return rows; }
    unsigned int getColums() const { return cols; }
    Vector3D getVector(unsigned int row) const
    {
        if (row > 3) throw Exception(Exception::MATRIX_OUT_RANGE);
        return Vector3D(matrix[row][0], matrix[row][1], matrix[row][2]);
    }
    double setIndex(unsigned int row, unsigned int col, double val)
    {
        if (row >= rows || col >= cols) throw Exception(Exception::MATRIX_OUT_RANGE);
        matrix[row][col] = val;
    }
    double getIndex(unsigned int row, unsigned int col) const
    {
        if (row >= rows || col >= cols) throw Exception(Exception::MATRIX_OUT_RANGE);
        return matrix[row][col];
    }
    double det()
    {
        if (rows == 2 && cols == 2) return det_2(0, 1, 0, 1);
        if (rows == 3 && cols == 3) return det_3(0, 1, 2, 0, 1, 2);
        else if (rows == 4 && cols == 4) { return det_4(0, 1, 2, 3, 0, 1, 2, 3); }
        else { throw Exception(Exception::MATRIX_SIZE_FOR_DET); }
    }
    ExtMatrix3D& operator=(const ExtMatrix3D &m)
    {
        if (this == &m) return *this;
        rows = m.rows; cols = m.cols;
        for (unsigned int row = 0; row < rows; row++)
            for (unsigned int col = 0; col < cols; col++) matrix[row][col] = m.matrix[row][col];
        return *this;
    }
    Vector3D operator*(const Vector3D &v) const
    {
        if (rows != 3 || cols != 3) throw Exception(Exception::MATRIX_SIZE_FOR_MULTI);
        return Vector3D(getVector(0).scalar_multi(v), getVector(1).scalar_multi(v), getVector(2).scalar_multi(v));
    }
    friend ostream& operator<<(ostream &, const ExtMatrix3D &);
};

ostream& operator<<(ostream &out, const ExtMatrix3D &m)
{
    for (unsigned int row = 0; row < m.rows; row++)
    {
        out << "|";
        for (unsigned int col = 0; col < m.cols; col++)
        {
            out.width(3);
            out << m.matrix[row][col] << (col == m.cols-1 ? "" : " ");
        }
        out << "|" << (row == m.rows-1 ? "" : "\n");
    }
    return out;
}

#endif // EXT_MATRIX_3D_H
