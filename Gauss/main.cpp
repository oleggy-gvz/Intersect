#include "Segment3D.h"

#define MATRIX_4x4

#ifdef MATRIX_4x4
#include "Matrix4x4\SysLinearEquat3D.h"
#else
#include "Matrix3D\SysLinearEquat3D.h"
#endif

#ifdef MATRIX_4x4

int Intersect(const Segment3D &s1, const Segment3D &s2, Vector3D &point)
{
    if (s1.isCollinearity(s2)) return -1; // lines belong to a common line
    if (s1.isParallel(s2)) return -2; // lines is parallel
    if (!s1.isCoplanarity(s2)) return 2; // not belong to a common surface

    Vector3D a = s1.getDirection(), A = s1.getStart();
    Vector3D b = s2.getDirection(), B = s2.getStart();
    double matrix[4][4] = {
                        {a.getY(),  -a.getX(),          0,    a.getY() * A.getX() - a.getX() * A.getY()},
                        {       0,  a.getZ(),   -a.getY(),    a.getZ() * A.getY() - a.getY() * A.getZ()},
                        {b.getY(),  -b.getX(),          0,    b.getY() * B.getX() - b.getX() * B.getY()},
                        {       0,  b.getZ(),   -b.getY(),    b.getZ() * B.getY() - b.getY() * B.getZ()}};
    Matrix4x4 M(matrix);
    SystemLinearEquations3D_SolutionGauss intersect(M);

    double res[4];
    unsigned int size;
    if (!intersect.calculateSolution(res, size)) return 2;
    if (size < 3) return 3;
    for (unsigned int i = 0; i < size; i++) point.setParam(i, res[i]);

    if (!s1.isLyingOnSegment(point) || !s2.isLyingOnSegment(point)) return 1;

    return 0;
}

#else

int Intersect(const Segment3D &s1, const Segment3D &s2, Vector3D &point)
{
    if (s1.isCollinearity(s2)) return -1; // lines belong to a common line
    if (s1.isParallel(s2)) return -2; // lines is parallel
    if (!s1.isCoplanarity(s2)) return 2; // not belong to a common surface

    Vector3D a = s1.getDirection(), A = s1.getStart();
    Vector3D b = s2.getDirection(), B = s2.getStart();
    Matrix3D M({a.getY(), -a.getX(),         0},
               {       0,  a.getZ(), -a.getY()},
               {b.getY(), -b.getX(),         0},
               {       0,  b.getZ(), -b.getY()});
    double res[4] = {a.getY() * A.getX() - a.getX() * A.getY(),
                   a.getZ() * A.getY() - a.getY() * A.getZ(),
                   b.getY() * B.getX() - b.getX() * B.getY(),
                   b.getZ() * B.getY() - b.getY() * B.getZ()};

    unsigned int size;
    SystemLinearEquations3D_SolutionGauss intersect(M, res);

    if (!intersect.calculateSolution(point, size)) return 2;
    if (size < 3) return 3;

    if (!s1.isLyingOnSegment(point) || !s2.isLyingOnSegment(point)) return 1;

    return 0;
}

#endif

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
        case 3: cout << "no intersect: non-3D case" << endl; break;
        case -1: cout << "no solutions: lines belong to a common line" << endl; break;
        case -2: cout << "no solutions: lines is parallel" << endl; break;
        }
    }
}

// -------------------------------------------------------------------

int main()
{
    cout << "Gauss method";
#ifdef MATRIX_4x4
    cout << " (use Matrix 4*4)" << endl;
#else
    cout << " (use Matrix3D)" << endl;
#endif
    cout << "***********************************" << endl << endl;

    Segment3D s1 = {{1, 1, 1}, {2, 2, 2}}, s2 = {{-3, -3, -3}, {-4, -4, -4}};
    printResultIntersect(s1, s2);
    cout << endl;

    Vector3D p = {2.1, 2.1, 2.1};
    if (s1.isLyingOnSegment(p)) cout << p << " - lying on segment: " << s1 << endl;
    else cout << p << " - not lying on segment: " << s1 << endl;
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

    Vector3D A = {1, 2, 1}; Vector3D vec_a = {3, 1, 4}; s1 = {A, A + vec_a};
    Vector3D B = {1, 2, 4}; Vector3D vec_b = {3, 1, 1}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;

    A = {3, -3, 2}; vec_a = {-1, 1, 2}; s1 = {A, A + vec_a};
    B = {-1, 4, -26}; vec_b = {3, -4, 6}; s2 = {B, B + vec_b};
    printResultIntersect(s1, s2);
    cout << endl;

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
