#ifndef MATRIX_4x4_H
#define MATRIX_4x4_H

#include "NumComp.h"
#include "Exeption.h"
#include <iostream>

//using namespace std;

class Matrix4x4
{
private:
    double matrix[4][4] = {{0}};
    unsigned int rows;
    unsigned int cols;

    double det_2(unsigned int i1, unsigned int i2, unsigned int j1, unsigned int j2) { return matrix[i1][j1] * matrix[i2][j2] - matrix[i2][j1] * matrix[i1][j2]; }
    double det_3(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int j1, unsigned int j2, unsigned int j3)
    {
        return matrix[i1][j1] * det_2(i2, i3, j2, j3) - matrix[i1][j2] * det_2(i2, i3, j1, j3) + matrix[i1][j3] * det_2(i2, i3, j1, j2);
    }
    double det_4(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4, unsigned int j1, unsigned int j2, unsigned int j3, unsigned int j4)
    {
        return matrix[i1][j1] * det_3(i2, i3, i4, j2, j3, j4) - matrix[i1][j2] * det_3(i2, i3, i4, j1, j3, j4) +
                matrix[i1][j3] * det_3(i2, i3, i4, j1, j2, j4) - matrix[i1][j4] * det_3(i2, i3, i4, j1, j2, j3);
    }

public:
    Matrix4x4(const Matrix4x4 &m) { *this = m; }
    Matrix4x4(double m[4][4]) { setMatrix(m); }
    void setMatrix(double m[4][4])
    {
        rows = 4; cols = 4;
        for (unsigned int row = 0; row < 4; row++)
            for (unsigned int col = 0; col < 4; col++) setIndex(row, col, m[row][col]);
    }
    unsigned int getRows() { return rows; }
    unsigned int getColums() { return cols; }
    void setIndex(unsigned int row, unsigned int col, double val)
    {
        if (row >= rows || col >= cols) throw Exception(Exception::MATRIX_OUT_RANGE);
        matrix[row][col] = equal_real(val, 0) ? 0 : val;
    }
    double getIndex(unsigned int row, unsigned int col)
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
    const Matrix4x4& operator=(const Matrix4x4 &m)
    {
        if (this == &m) return *this;
        rows = m.rows; cols = m.cols;
        for (unsigned int row = 0; row < rows; row++)
            for (unsigned int col = 0; col < cols; col++) matrix[row][col] = m.matrix[row][col];
        return *this;
    }
    friend ostream& operator<<(ostream &, const Matrix4x4 &);
};

ostream& operator<<(ostream &out, const Matrix4x4 &m)
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

#endif // MATRIX_4x4_H
