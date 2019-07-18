#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Exception: public exception
{
private:
    string m_error;
public:
    enum ExceptionType { VECTOR3D_OUT_RANGE, SEGMENT_EQUAL_POINTS, MATRIX3D_OUT_RANGE, MATRIX3D_SIZE_FOR_DET,
                         MATRIX_OUT_RANGE, MATRIX_SIZE_FOR_DET, MATRIX_SIZE_FOR_MULTI, EQUATIONS_NOT_RESULT };
    Exception(string error) : m_error(error) {}
    Exception(ExceptionType error)
    {
        switch(error)
        {
        case VECTOR3D_OUT_RANGE:    m_error = "Vector3D (out of range parameter of Vector3D)"; break;
        case SEGMENT_EQUAL_POINTS:  m_error = "Segment3D (start and end points of the segment must be different)"; break;
        case MATRIX3D_OUT_RANGE:    m_error = "Matrix3D (out of range)"; break;
        case MATRIX3D_SIZE_FOR_DET: m_error = "Matrix3D determinant (matrix size is not equal 3)"; break;
        case MATRIX_OUT_RANGE:      m_error = "Matrix (out of range)"; break;
        case MATRIX_SIZE_FOR_DET:   m_error = "Matrix (matrix size is not equal 3 or 4)"; break;
        case MATRIX_SIZE_FOR_MULTI: m_error = "Matrix multiply Vector3D (matrix size is not equal 3)"; break;
        case EQUATIONS_NOT_RESULT:  m_error = "LinearEquations (number of equations is not equal to number of results)"; break;
        }
    }
    const char* what() const noexcept { return m_error.c_str(); }
};

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

public:
    Vector3D() : X(0), Y(0), Z(0) {}
    Vector3D(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {}
    Vector3D(const Vector3D &v) { *this = v; }
    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }
    double getParam(unsigned int index) const
    {
        switch(index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw Exception(Exception::VECTOR3D_OUT_RANGE);
        }
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

ostream& operator<<(ostream &out, const Vector3D &v) { out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")"; return out; }

// -------------------------------------------------------------------

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

public:
    Segment3D(const Vector3D &_start, const Vector3D &_end) : start(_start), end(_end)
    {
        if (getDirection() == Vector3D(0, 0 ,0)) throw Exception(Exception::SEGMENT_EQUAL_POINTS);
    }
    bool isCollinearity(const Segment3D &s) const // belong to a line of segment
    {
        Vector3D vec1 = s.start - start;
        Vector3D vec2 = s.end - start;
        Vector3D dir = getDirection();
        return vec1.isCollinearity(dir) && vec2.isCollinearity(dir);
    }
    bool isParallel(const Segment3D &s) const { return getDirection().isCollinearity(s.getDirection()); }
    Vector3D getStart() const { return start; }
    Vector3D getEnd() const { return end; }
    Vector3D getDirection() const { return end - start; }
    bool isCoplanarity(const Segment3D &line) const // belong to a common 2D surface
    {
        Vector3D vec = line.start - start;
        return equal_real(vec.mixed_multi(getDirection(), line.getDirection()), 0);
    }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s) { out << s.start << " -> " << s.end; return out; }

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
    void setMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &v4){ rows = 4; cols = 3; vectors[0] = v1; vectors[1] = v2; vectors[2] = v3; vectors[3] = v4; }
    unsigned int getRows() const { return rows; }
    unsigned int getColums() const { return cols; }
    void setRow(unsigned int row, const Vector3D &v) { if (row >= rows) throw Exception(Exception::MATRIX3D_OUT_RANGE); vectors[row] = v; }
    Vector3D getRow(unsigned int row) const { if (row >= rows) throw Exception(Exception::MATRIX3D_OUT_RANGE); return vectors[row]; }
    double getIndex(unsigned int row, unsigned int col) const { if (row >= rows || col >= cols) throw Exception(Exception::MATRIX3D_OUT_RANGE); return vectors[row].getParam(col); }
    double det() { if (rows != 3 || cols != 3) throw Exception(Exception::MATRIX3D_SIZE_FOR_DET); return vectors[0].mixed_multi(vectors[1], vectors[2]); }
    Matrix3D& operator=(const Matrix3D &m)
    {
        if (this == &m) return *this;
        rows = m.rows; cols = m.cols;
        for (int i = 0; i < rows; i++) vectors[i] = m.vectors[i];
        return *this;
    }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(vectors[0].scalar_multi(v), vectors[1].scalar_multi(v), vectors[2].scalar_multi(v)); }
    friend ostream& operator<<(ostream &, const Matrix3D &);
};

ostream& operator<<(ostream &out, const Matrix3D &m)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        out << "|";
        for (unsigned int j = 0; j < 3; j++)
        {
            out.width(3);
            out << m.getIndex(i, j) << (j == 2 ? "" : " ");
        }
        out << "|" << (i == 2 ? "" : "\n");
    }
    return out;
}

class ExtMatrix3D
{
private:
    double matrix[4][4];
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
        for (unsigned int col = 0; col < cols; col++) { matrix[0][col] = v1.getParam(col); matrix[1][col] = v2.getParam(col); matrix[2][col] = v3.getParam(col); }
    }
    void setMatrix(const Vector3D v[4], double b[4])
    {
        rows = 4; cols = 4;
        for (unsigned int row = 0; row < rows; row++)
            for (unsigned int col = 0; col < 3; col++) matrix[row][col] = v[row].getParam(col);
        for (unsigned int row = 0; row < rows; row++) matrix[row][rows-1] = b[4];
    }
    unsigned int getRows() const { return rows; }
    unsigned int getColums() const { return cols; }
    Vector3D getVector(int row) const
    {
        if (row > 3) throw Exception(Exception::MATRIX_OUT_RANGE);
        return Vector3D(matrix[row][0], matrix[row][1], matrix[row][2]);
    }
    double getIndex(unsigned int row, unsigned int col) const { if (row >= rows || col >= cols) throw Exception(Exception::MATRIX_OUT_RANGE); return matrix[row][col]; }
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
    for (unsigned int i = 0; i < m.rows; i++)
    {
        out << "|";
        for (unsigned int j = 0; j < m.cols; j++)
        {
            out.width(3);
            out << m.matrix[i][j] << (j == m.cols-1 ? "" : " ");
        }
        out << "|" << (i == m.rows-1 ? "" : "\n");
    }
    return out;
}

class SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
protected:
    Matrix3D ratios;
    Vector3D solution;
    double result[4] = {0};

public:
    SystemLinearEquations3D(const Matrix3D &_ratios, double *_result) : ratios(_ratios)
    {
        for (unsigned int row = 0; row < ratios.getRows(); row++)
            result[row] = _result[row];
    }
    virtual bool calcSolution() = 0;
    Vector3D getSolution()
    {
        return solution;
    }
};

class SystemLinearEquations3D_SolutionGauss : public SystemLinearEquations3D // solving a system of linear equations: ratios * x = result
{
private:
    bool sortRowsByZero(unsigned int col, unsigned int start_row)
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
    unsigned int transformDiagonalView() // transform the matrix to a diagonal view and return rank of matrix ratios
    {
        double ratio;
        unsigned int rank = 0; // ratios matrix rank
        // transform the matrix to a diagonal view
        for (unsigned int col = 0; col < ratios.getColums(); col++)
        {
            if (!sortRowsByZero(col, rank)) continue;
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
        // divide each row of matrix on by first nonzero element
        for (unsigned int row = 0; row < ratios.getRows(); row++)
        {
            for (unsigned int col = 0; col < ratios.getColums(); col++)
            {
                double div = ratios.getIndex(row, col);
                if (!equal_real(div, 0))
                {
                    ratios.setRow(row, ratios.getRow(row) / div);
                    result[row] /= div;
                    break;
                }
            }
        }
        return rank;
    }
public:
    SystemLinearEquations3D_SolutionGauss(const Matrix3D &_ratios, double *_result) : SystemLinearEquations3D(_ratios, _result) {}
    bool calcSolution()
    {
        unsigned int rank = transformDiagonalView();
        for (unsigned int row = rank; row < ratios.getRows(); row++)
            if (!equal_real(result[row], 0)) return false; // rank (ratios) != rank (ratios | result)

        for (unsigned int i = 0; i < rank; i++) // number of unknowns == rank (ratios)
        {
            unsigned int row = rank-1 - i; // row = rank-1...0
            unsigned int col_start = ratios.getColums() - rank + row;
            double unknown = result[row];
            for (unsigned int col = col_start + 1; col < ratios.getColums(); col++) // col = (cols-rank+row+1)...cols
                unknown -= ratios.getIndex(row, col) * solution.getParam(col);
            solution.setParam(col_start, unknown);
        }
        return true;
    }
};

int Intersect(const Segment3D &s1, const Segment3D &s2, Vector3D &point)
{
    if (s1.isCollinearity(s2)) return -1; // lines belong to a common line
    if (s1.isParallel(s2)) return -2; // lines is parallel
    if (!s1.isCoplanarity(s2)) return 2; // not belong to a common surface

    Vector3D a = s1.getDirection(), A = s1.getStart();
    Vector3D b = s2.getDirection(), B = s2.getStart();
    Matrix3D M({a.getY(), -a.getX(),          0},
               {       0,  a.getZ(), -a.getY()},
               {b.getY(), -b.getX(),          0},
               {       0,  b.getZ(), -b.getY()});
    double res[4] = {a.getY() * A.getX() - a.getX() * A.getY(),
                   a.getZ() * A.getY() - a.getY() * A.getZ(),
                   b.getY() * B.getX() - b.getX() * B.getY(),
                   b.getZ() * B.getY() - b.getY() * B.getZ()};

    SystemLinearEquations3D_SolutionGauss intersect(M, res);

    if (intersect.calcSolution()) point = intersect.getSolution();
    else return 2;

    /*if (!s1.isInsideSegment(point) || !s2.isInsideSegment(point)) return 1;*/

    return 0;
}

void printResultIntersect(const Segment3D &s1, const Segment3D &s2)
{
    Vector3D point;
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;
    int res = Intersect(s1, s2, point);
    if (res == 0) cout << "point of intersection is " << point << endl;
    else
    {
        switch(res)
        {
        case 1: cout << "no intersect: point of intersection " << point << " is outside the boundaries of the segments" << endl; break;
        case 2: cout << "no intersect: lines is not belong to a common surface" << endl; break;
        case -1: cout << "no solutions: lines belong to a common line" << endl; break;
        case -2: cout << "no solutions: lines is parallel" << endl; break;
        }
    }
}

// -------------------------------------------------------------------

int main()
{
    Segment3D s1 = {{1, 1, 0}, {2, 2, 0}}, s2 = {{-3, -3, 0}, {-4, -4, 0}};
    /*printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{2, 1, 0}, {3, 2, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{1, 2, 0}, {2, 1, 0.1}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{1, 1, 0}, {2, 2, 0}};
    s2 = {{1, 2, 0}, {2, 1, 0}};
    printResultIntersect(s1, s2);
    cout << endl;

    s1 = {{0, 0.5, 2}, {0, 0.5, 1}};
    s2 = {{0, 2, 0.5}, {0, 1, 0.5}};
    printResultIntersect(s1, s2);
    cout << endl;*/

    Vector3D A = {3, -3, 2}; Vector3D vec_a = {-1, 1, 2}; s1 = {A, A + vec_a};
    Vector3D B = {-1, 4, -26}; Vector3D vec_b = {3, -4, 6}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;

    /*Matrix3D M1({6, 1, 2},
               {4, -6, 16},
               {3, 8, 1});

    Matrix3D M(M1);
    double b[] = {21, 2, 2};

    Vector3D x_check = {62/15.0, -17/15.0, -4/3.0};

    cout << "M = " << endl << M << endl;
    cout << "x (check) = " << x_check << endl;

    Vector3D b_check = M * x_check;
    cout << "b (check) = " << b_check << endl;

    SystemLinearEquations3D_SolutionGauss R(M, b);
    if (R.calcSolution())
    {
        Vector3D x = R.getSolution();
        cout << "x = " << x << endl;
    }*/


    /*Vector3D a = {1, 2, 3};
    cout << "a = " << a << endl;
    cout << endl;

    Vector3D b = a;
    cout << "b = " << b << endl;
    cout << endl;

    Vector3D c = a + b;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;*/

    return 0;
}
