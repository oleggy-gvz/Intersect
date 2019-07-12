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
    Vector3D(const Vector3D &v) : X(v.getX()), Y(v.getY()), Z(v.getZ()) {}
    double getX() const { return X; }
    double getY() const { return Y; }
    double getZ() const { return Z; }
    double getParam(int i) const
    {
        switch(i)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
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
    Vector3D operator=(const Vector3D &v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    Vector3D operator+(const Vector3D &v) const { return Vector3D(X+v.X, Y+v.Y, Z+v.Z); }
    Vector3D operator-(const Vector3D &v) const { return Vector3D(X-v.X, Y-v.Y, Z-v.Z); }
    Vector3D operator*(double k) const { return Vector3D(X * k, Y * k, Z * k); }
    Vector3D operator/(double k) const { return Vector3D(X / k, Y / k, Z / k); }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(Y * v.Z - v.Y * Z, -X * v.Z + v.X * Z, X * v.Y - v.X * Y); }
    bool operator==(const Vector3D &v) { return equal_real(X, v.X) && equal_real(Y, v.Y) && equal_real(Z, v.Z); }
    friend ostream & operator<<(ostream &, const Vector3D &);
};

ostream & operator<<(ostream &out, const Vector3D &v) { out << "(" << v.X << ", " << v.Y << ", " << v.Z << ")"; return out; }

// -------------------------------------------------------------------

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    /*struct Ratio { double x = 0, y = 0, z = 0; bool isX = true, isY = true, isZ = true; }; // bool - no axis

    Vector3D getVectorFromRario(Ratio ratio) const
    {
        Vector3D res;
        Vector3D dir = getDirection();
        if (ratio.isX)      res = start + dir * ratio.x;
        else if (ratio.isY) res = start + dir * ratio.y;
        else if (ratio.isZ) res = start + dir * ratio.z;
        else                res = start + dir * ratio.x; // any axis
        return res;
    }

    Ratio getRatio(const Vector3D &point) const
    {
        Vector3D diff = point - start;
        Vector3D dir = getDirection();
        Ratio ratio;

        ratio.isX = !equal_real(dir.getX(), 0);
        ratio.x = ratio.isX ? diff.getX() / dir.getX() : -1;
        ratio.isY = !equal_real(dir.getY(), 0);
        ratio.y = ratio.isY ? diff.getY() / dir.getY() : -1;
        ratio.isZ = !equal_real(dir.getZ(), 0);
        ratio.z = ratio.isZ ? diff.getZ() / dir.getZ() : -1;
        return ratio;
    }

    bool isInsideSegment(const Vector3D &point) const
    {
        Ratio ratio = getRatio(point);
        Ratio ratio_start = getRatio(start);
        Ratio ratio_end = getRatio(end);
        bool res;
        if (ratio.isX)      res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x;
        else if (ratio.isY) res = ratio_start.y <= ratio.y && ratio.y <= ratio_end.y;
        else if (ratio.isZ) res = ratio_start.z <= ratio.z && ratio.z <= ratio_end.z;
        else                res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x; // any axis
        return res;
    }

    bool ratioIntersect(double a1, double a2, double b1, double b2, double dif1, double dif2, double &ratio) const
    {
        bool res = false;
        ratio = -1;
        if (!equal_real(a2 * b1, a1 * b2)) // check divide by zero
        {
            res = true;
            ratio = (a2 * dif1 - a1 * dif2) / (a2 * b1 - a1 * b2);
        }
        return res;
    }

    Ratio getRatioIntersect(const Segment3D &line) const
    {
        Vector3D s1_v = getDirection();
        Vector3D s2_v = line.getDirection();
        Vector3D dif = start - line.start;
        Ratio ratio;
        ratio.isX = ratioIntersect(s1_v.getY(), s1_v.getZ(), s2_v.getY(), s2_v.getZ(), dif.getY(), dif.getZ(), ratio.x);
        ratio.isY = ratioIntersect(s1_v.getX(), s1_v.getZ(), s2_v.getX(), s2_v.getZ(), dif.getX(), dif.getZ(), ratio.y);
        ratio.isZ = ratioIntersect(s1_v.getX(), s1_v.getY(), s2_v.getX(), s2_v.getY(), dif.getX(), dif.getY(), ratio.z);
        return ratio;
    }*/
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
    unsigned int row;
    unsigned int col;

public:
    Matrix3D(const Matrix3D &m) : row(m.row), col(m.col) { for (unsigned int i = 0; i < 3; i++) vectors[i] = m.vectors[i]; }
    Matrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { setMatrix3D(v1, v2, v3); }
    Matrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &v4) { setMatrix3D(v1, v2, v3, v4); }
    void setMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { row = 3; col = 3; vectors[0] = v1; vectors[1] = v2; vectors[2] = v3; }
    void setMatrix3D(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &v4){ row = 4; col = 3; vectors[0] = v1; vectors[1] = v2; vectors[2] = v3; vectors[3] = v4; }
    unsigned int getRow() const { return row; }
    unsigned int getColum() const { return col; }
    Vector3D getVector(unsigned int _row) const
    {
        if (_row > row-1) throw Exception(Exception::MATRIX3D_OUT_RANGE);
        return vectors[_row];
    }
    virtual double getIndex(unsigned int i, unsigned int j) const
    {
        if (i >= row || j >= col) throw Exception(Exception::MATRIX3D_OUT_RANGE);
        return vectors[i].getParam(j);
    }
    double det()
    {
        if (row != 3 || col != 3) throw Exception(Exception::MATRIX3D_SIZE_FOR_DET);
        return vectors[0].mixed_multi(vectors[1], vectors[2]);
    }
    Matrix3D operator=(const Matrix3D &m) { for (int i = 0; i < 3; i++) vectors[i] = m.vectors[i]; return *this; }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(vectors[0].scalar_multi(v), vectors[1].scalar_multi(v), vectors[2].scalar_multi(v)); }
    friend ostream& operator<<(ostream &, const Matrix3D &);
};

ostream& operator<<(ostream &out, const Matrix3D &m)
{
    for (int i = 0; i < 3; i++)
    {
        out << "|";
        for (int j = 0; j < 3; j++)
        {
            out.width(3);
            out << m.getIndex(i, j) << (j == 2 ? "" : " ");
        }
        out << "|" << (i == 2 ? "" : "\n");
    }
    return out;
}

class Matrix
{
private:
    double matrix[4][4];
    int row;
    int col;

    double det_2(int i1, int i2,int j1, int j2) { return matrix[i1][j1] * matrix[i2][j2] - matrix[i2][j1] * matrix[i1][j2]; }
    double det_3(int i1, int i2, int i3, int j1, int j2, int j3)
    {
        return matrix[i1][j1] * det_2(i2, i3, j2, j3) - matrix[i1][j2] * det_2(i2, i3, j1, j3) + matrix[i1][j3] * det_2(i2, i3, j1, j2);
    }

public:
    Matrix(const Matrix &m) : row(m.row), col(m.col)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++) matrix[i][j] = m.matrix[i][j];
    }
    Matrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3) { setMatrix(v1, v2, v3); }
    Matrix(const Vector3D v[4], double b[4]) { setMatrix(v, b); }
    void setMatrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
    {
        row = 3; col = 3;
        for (int j = 0; j < col; j++) { matrix[0][j] = v1.getParam(j); matrix[1][j] = v2.getParam(j); matrix[2][j] = v3.getParam(j); }
    }
    void setMatrix(const Vector3D v[4], double b[4])
    {
        row = 4; col = 4;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++) matrix[i][j] = v[i].getParam(j);
        for (int i = 0; i < row; i++) matrix[i][row-1] = b[4];
    }
    int getRow() const { return row; }
    int getColum() const { return col; }
    Vector3D getVector(int row) const
    {
        if (row > 3) throw Exception(Exception::MATRIX_OUT_RANGE);
        return Vector3D(matrix[row][0], matrix[row][1], matrix[row][2]);
    }
    double det()
    {
        if (row == 3 && col == 3) return det_3(0, 1, 2, 0, 1, 2);
        else if (row == 4 && col == 4)
        {
            return matrix[0][0] * det_3(1, 2, 3, 1, 2, 3) - matrix[0][1] * det_3(1, 2, 3, 0, 2, 3) +
                    matrix[0][2] * det_3(1, 2, 3, 0, 1, 3) - matrix[0][3] * det_3(1, 2, 3, 0, 1, 2);
        }
        else
        {
            throw Exception(Exception::MATRIX_SIZE_FOR_DET);
        }
    }
    Matrix operator=(const Matrix &m)
    {
        row = m.row; col = m.col;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++) matrix[i][j] = m.matrix[i][j];
        return *this;
    }
    Vector3D operator*(const Vector3D &v) const
    {
        if (row != 3 || col != 3) throw Exception(Exception::MATRIX_SIZE_FOR_MULTI);
        return Vector3D(getVector(0).scalar_multi(v), getVector(1).scalar_multi(v), getVector(2).scalar_multi(v));
    }
    friend ostream& operator<<(ostream &, const Matrix &);
};

ostream& operator<<(ostream &out, const Matrix &m)
{
    for (int i = 0; i < m.row; i++)
    {
        out << "|";
        for (int j = 0; j < m.col; j++)
        {
            out.width(3);
            out << m.matrix[i][j] << (j == m.col-1 ? "" : " ");
        }
        out << "|" << (i == m.row-1 ? "" : "\n");
    }
    return out;
}

class LinearEquations // solving a system of linear equations: ratios * x = result
{
private:
    Matrix3D ratios;
    double result[4];

    bool isFindRowNotNull(int col, int &row)
    {
        int res = false; row = -1;
        for (int n = 0; n < ratios.getRow(); n++)
            if (!equal_real(ratios.getIndex(n, col), 0)) { res = true; row = n; }
        return res;
    }
public:
    LinearEquations(const Matrix3D &_ratios, double *_result) : ratios(_ratios)
    {
        for (int row = 0; row < ratios.getRow(); row++) result[row] = _result[row];
    }
    LinearEquations(const Matrix3D &_ratios, Vector3D &_result) : ratios(_ratios)
    {
        if (ratios.getRow() != 3) throw Exception(Exception::EQUATIONS_NOT_RESULT);
        for (int row = 0; row < ratios.getRow(); row++) result[row] = _result.getParam(row);
    }
    Vector3D getSolutionOnGauss()
    {
        int row;
        if(!isFindRowNotNull(0, row)) cout << "no" << endl;
        cout << row << endl;

        return Vector3D(0, 0, 0);
    }
};

int Intersect(const Segment3D &s1, const Segment3D &s2, Vector3D &cross)
{
    if (s1.isCollinearity(s2)) return -1; // lines belong to a common line
    if (s1.isParallel(s2)) return -2; // lines is parallel
    if (!s1.isCoplanarity(s2)) return 2; // not belong to a common surface

    /*Ratio ratio = getRatioIntersect(s2);
    cross = line.getVectorFromRario(ratio);
    if (!s1.isInsideSegment(cross) || !s2.isInsideSegment(cross)) return 1;*/

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
    /*Segment3D s1 = {{1, 1, 0}, {2, 2, 0}}, s2 = {{-3, -3, 0}, {-4, -4, 0}};
    printResultIntersect(s1, s2);
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
    cout << endl;

    Vector3D A = {3, -3, 2}; Vector3D vec_a = {-1, 1, 2}; s1 = {A, A + vec_a};
    Vector3D B = {-1, 4, -26}; Vector3D vec_b = {3, -4, 6}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;*/


    Matrix3D M({6, 1, 2},
               {4, -6, 16},
               {3, 8, 1});

    Matrix3D M1({0, 1, 2},
               {0, -6, 16},
               {1, 8, 1});

    Vector3D b = {21, 2, 2};
    Vector3D x_check = {62/15.0, -17/15.0, -4/3.0};

    /*cout << "M = " << endl << M << endl;
    cout << "b = " << b << endl;
    cout << "x = " << x_check << endl;

    Vector3D b_check = M * x_check;
    cout << "b (check) = " << b_check << endl;*/

    M = M1;
    LinearEquations R(M, b);
    Vector3D x = R.getSolutionOnGauss();
    //cout << "x (check) = " << x << endl;

    return 0;
}
