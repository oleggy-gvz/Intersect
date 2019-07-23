#ifndef SYSTEM_LINEAR_EQUATIONS_3D_H
#define SYSTEM_LINEAR_EQUATIONS_3D_H

#include "Matrix4x4.h"

class SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
protected:
    Matrix4x4 ratios;

public:
    SystemLinearEquations3D(const Matrix4x4 &_ratios) : ratios(_ratios) {}
    virtual bool calculateSolution(double *solution, unsigned int &size) = 0;
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

        unsigned int size = ratios.getColums();
        double tmp_row[4];

        for (unsigned int col = 0; col < size; col++) tmp_row[col] = ratios.getIndex(row1, col);
        for (unsigned int col = 0; col < size; col++) ratios.setIndex(row1, col, ratios.getIndex(row2, col));
        for (unsigned int col = 0; col < size; col++) ratios.setIndex(row2, col, tmp_row[col]);
    }
    unsigned int directAction() // make all coefficients equal to zero which are below the main diagonal
    {
        double ratio;
        unsigned int rank = 0; // ratios matrix rank, equal to number of rows with unknowns
        for (unsigned int col = 0; col < ratios.getColums() - 1; col++) // row = 0..cols-1
        {
            if (!sortRowsByNonZeroElementColum(col, rank)) continue;
            for (unsigned int row = rank + 1; row < ratios.getRows(); row++)
            {
                double a = ratios.getIndex(row, col);
                if (equal_real(a, 0)) continue;
                ratio = -a / ratios.getIndex(rank, col); // -a21/a11,-a31/a11,-a41/a11,..

                for (unsigned int j = 0; j < ratios.getColums(); j++)
                {
                    double new_ratio = ratios.getIndex(row, j) + ratios.getIndex(rank, j) * ratio;
                    new_ratio = equal_real(new_ratio, 0) ? 0 : new_ratio;
                    ratios.setIndex(row, j, new_ratio);
                }
            }
            rank++;
        }
        return rank;
    }
    void reversAction(unsigned int rank) // make all coefficients equal to zero which are higher the main diagonal
    {
        if (rank < 2) return;
        double ratio;
        for (unsigned int col = ratios.getColums() - 2; col > 0; col--)
        {
            for (unsigned int row = rank-2; row < rank; row--) // row = rank-2..0
            {
                 double a = ratios.getIndex(row, col);
                 if (equal_real(a, 0)) continue;
                 ratio = -a / ratios.getIndex(rank-1, col);
                 for (unsigned int j = 0; j < ratios.getColums(); j++)
                 {
                     double new_ratio = ratios.getIndex(row, j) + ratios.getIndex(rank-1, j) * ratio;
                     new_ratio = equal_real(new_ratio, 0) ? 0 : new_ratio;
                     ratios.setIndex(row, j, new_ratio);
                 }
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

            for (unsigned int j = i; j < ratios.getColums(); j++)
                ratios.setIndex(i, j, ratios.getIndex(i, j) / led_elem);
        }
    }
public:
    SystemLinearEquations3D_SolutionGauss(const Matrix4x4 &_ratios) : SystemLinearEquations3D(_ratios) {}

    bool calculateSolution(double *solution, unsigned int &size)
    {
        unsigned int rank = directAction();
        reversAction(rank);
        normalizeMainDiagonal();

        for (unsigned int row = rank; row < ratios.getRows(); row++)
        {
            double res = ratios.getIndex(row, ratios.getColums()-1);
            if (!equal_real(res, 0)) return false; // rank (ratios) != rank (ratios | result)
        }
        for (unsigned int row = 0; row < rank; row++) solution[row] = ratios.getIndex(row, ratios.getColums()-1);
        size = rank;

        return true;
    }
};

#endif // SYSTEM_LINEAR_EQUATIONS_3D_H
