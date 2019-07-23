#ifndef SYSTEM_LINEAR_EQUATIONS_3D_H
#define SYSTEM_LINEAR_EQUATIONS_3D_H

#include "Matrix3D.h"

class SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
protected:
    Matrix3D ratios;
    double result[4] = {0};

public:
    SystemLinearEquations3D(const Matrix3D &_ratios, double res[4]) : ratios(_ratios)
    {
        for (unsigned int row = 0; row < ratios.getRows(); row++)
            result[row] = res[row];
    }
    virtual bool calculateSolution(Vector3D &solution, unsigned int &size) = 0;
};

class SystemLinearEquations3D_SolutionGauss : public SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
private:
    bool sortRowsByNonZeroElementColum(unsigned int col, unsigned int start_row)
    {
        for (unsigned int row = start_row; row < ratios.getRows(); row++) // row = start_row..rows
            if (!equal_real(ratios.getIndex(row, col), 0)) { swapRows(row, start_row); return true; }
        return false; // all zero elements in column
    }
    void swapRows(unsigned int row1, unsigned int row2)
    {
        if (row1 == row2) return;
        Vector3D tmp_v = ratios.getRow(row1); ratios.setRow(row1, ratios.getRow(row2)); ratios.setRow(row2, tmp_v);
        double tmp = result[row1]; result[row1] = result[row2]; result[row2] = tmp;
    }
    unsigned int directAction() // transform the matrix to a diagonal view and return rank of matrix ratios
    {
        double ratio;
        unsigned int rank = 0; // ratios matrix rank, equal to number of rows with unknowns
        for (unsigned int col = 0; col < ratios.getColums(); col++)
        {
            if (!sortRowsByNonZeroElementColum(col, rank)) continue;
            for (unsigned int row = rank + 1; row < ratios.getRows(); row++)
            {
                double a = ratios.getIndex(row, col);
                if (equal_real(a, 0)) continue;
                ratio = -a / ratios.getIndex(rank, col); // -a21/a11,-a31/a11,-a41/a11,..
                ratios.setRow(row, ratios.getRow(row) + ratios.getRow(rank) * ratio);
                result[row] += result[rank] * ratio;
            }
            rank++;
        }
        return rank;
    }
    void reversAction(unsigned int rank)
    {
        if (rank < 2) return;
        double ratio;
        for (unsigned int col = ratios.getColums() - 1; col > 0; col--)
        {
            for (unsigned int row = rank-2; row < rank; row--) // row = rank-2..0
            {
                 double a = ratios.getIndex(row, col);
                 if (equal_real(a, 0)) continue;
                 ratio = -a / ratios.getIndex(rank-1, col);
                 ratios.setRow(row, ratios.getRow(row) + ratios.getRow(rank-1) * ratio);
                 result[row] += result[rank-1] * ratio;
            }
            rank--;
        }
    }
    void normalizeMainDiagonal() // make main diagonal of matrix are equal to 1
    {
        for (unsigned int i = 0; i < ratios.getRows() && i < ratios.getColums(); i++)
        {
            double led_elem = ratios.getIndex(i, i); // divide each row of matrix on by lead element
            if (equal_real(led_elem, 0) || equal_real(led_elem, 1)) continue;

            ratios.setRow(i, ratios.getRow(i) / led_elem);
            result[i] /= led_elem;
        }
    }
public:
    SystemLinearEquations3D_SolutionGauss(const Matrix3D &_ratios, double *_result) : SystemLinearEquations3D(_ratios, _result) {}
    bool calculateSolution(Vector3D &solution, unsigned int &size)
    {
        unsigned int rank = directAction();
        reversAction(rank);
        normalizeMainDiagonal();

        for (unsigned int row = rank; row < ratios.getRows(); row++)
            if (!equal_real(result[row], 0)) return false; // rank (ratios) != rank (ratios | result)

        for (unsigned int row = 0; row < rank; row++) solution.setParam(row, result[row]);
        size = rank;

        return true;
    }
};

#endif // SYSTEM_LINEAR_EQUATIONS_3D_H
