#include "Segment3Dext.h"

using namespace std;

// -------------------------------------------------------------------

void printResultIntersect(const Segment3D &s1, const Segment3D &s2)
{
    Vector3D point;
    cout << "segment 1 = " << s1 << endl;
    cout << "segment 2 = " << s2 << endl;
    Segment3D_Ext s1_ext(s1);
    int res = s1_ext.Intersect(s2, point);
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
    cout << "Parametric method" << endl;
    cout << "***********************************" << endl << endl;

    Segment3D s1 = {{1, 1, 1}, {2, 2, 2}}, s2 = {{-3, -3, -3}, {-4, -4, -4}};
    printResultIntersect(s1, s2);
    cout << endl;

    Vector3D p = {1.9, 1.9, 1.9};
    Segment3D_Ext s1_ext(s1);
    if (s1_ext.isInsideSegment(p)) cout << p << " - lying on segment: " << s1 << endl;
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
