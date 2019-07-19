#ifndef SYSTEM_LINEAR_EQUATIONS_3D_H
#define SYSTEM_LINEAR_EQUATIONS_3D_H

#include "ExtMatrix3D.h"

class SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
protected:
    ExtMatrix3D ratios;

public:
    SystemLinearEquations3D(const ExtMatrix3D &_ratios) : ratios(_ratios) {}
    virtual bool calculateSolution(double *solution, unsigned int &size) = 0;
};

class SystemLinearEquations3D_SolutionGauss : public SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
private:
    bool sortRowsByElementColum(unsigned int col, unsigned int start_row)
    {
        for (unsigned int row = start_row; row < ratios.getRows(); row++) // row = start_row..rows
            if (!equal_real(ratios.getIndex(row, col), 0)) { swapRows(row, start_row); return true; }
        return false; // all zero elements in column
    }
    void swapRows(unsigned int row1, unsigned int row2)
    {
        if (row1 == row2) return;

        unsigned int size = ratios.getColums();
        double *tmp_row = new double[size];

        for (unsigned int col = 0; col < size; col++) tmp_row[col] = ratios.getIndex(row1, col);
        for (unsigned int col = 0; col < size; col++) ratios.setIndex(row1, col, ratios.getIndex(row2, col));
        for (unsigned int col = 0; col < size; col++) ratios.setIndex(row2, col, tmp_row[col]);

        delete []tmp_row;
    }
    unsigned int directAction() // transform the matrix to a diagonal view and return rank of matrix ratios
    {
        double ratio;
        unsigned int rank = 0; // ratios matrix rank, equal to number of rows with unknowns
        for (unsigned int col = 0; col < ratios.getColums() - 1; col++) // row = 0..cols-1
        {
            if (!sortRowsByElementColum(col, rank)) continue;
            for (unsigned int row = rank + 1; row < ratios.getRows(); row++)
            {
                double a = ratios.getIndex(row, col);
                if (equal_real(a, 0)) continue;
                ratio = -a / ratios.getIndex(rank, col); // -a21/a11,-a31/a11,-a41/a11,..

                for (unsigned int j = 0; j < ratios.getColums(); j++)
                    ratios.setIndex(row, j, ratios.getIndex(row, j) + ratios.getIndex(rank, j) * ratio);
            }
            rank++;
        }
        return rank;
    }
    void reversAction(unsigned int rank)
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
                     ratios.setIndex(row, j, ratios.getIndex(row, j) + ratios.getIndex(rank-1, j) * ratio);
            }
            rank--;
        }
    }
    void normalizeMainDiagonal() // make main diagonal of matrix are equal to 1
    {
        for (unsigned int row = 0; row < ratios.getRows()-1; row++)
        {
            for (unsigned int col = 0; col < ratios.getColums()-1; col++)
            {
                double div = ratios.getIndex(row, col); // divide each row of matrix on by first nonzero element
                if (!equal_real(div, 0))
                {
                    for (unsigned int j = 0; j < ratios.getColums(); j++)
                        ratios.setIndex(row, j, ratios.getIndex(row, j) / div);
                    break;
                }
            }
        }
    }
public:
    SystemLinearEquations3D_SolutionGauss(const ExtMatrix3D &_ratios) : SystemLinearEquations3D(_ratios) {}

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
        return true;

        for (unsigned int row = 0; row < rank; row++) solution[row] = ratios.getIndex(row, ratios.getColums()-1);
        size = rank;
    }
};

#endif // SYSTEM_LINEAR_EQUATIONS_3D_H
