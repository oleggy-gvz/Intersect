#ifndef EXEPTION_H
#define EXEPTION_H

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
        // for class Vector3D
        case VECTOR3D_OUT_RANGE:    m_error = "Vector3D (out of range parameter of Vector3D)"; break;
        // for class Segment3D
        case SEGMENT_EQUAL_POINTS:  m_error = "Segment3D (start and end points of the segment must be different)"; break;
        // for class Matrix3D
        case MATRIX3D_OUT_RANGE:    m_error = "Matrix3D (out of range)"; break;
        case MATRIX3D_SIZE_FOR_DET: m_error = "Matrix3D determinant (matrix size is not equal 3)"; break;
        // for class ExtMatrix3D
        case MATRIX_OUT_RANGE:      m_error = "ExtMatrix3D (out of range)"; break;
        case MATRIX_SIZE_FOR_DET:   m_error = "ExtMatrix3D (matrix size is not equal 3 or 4)"; break;
        case MATRIX_SIZE_FOR_MULTI: m_error = "ExtMatrix3D multiply Vector3D (matrix size is not equal 3)"; break;
        // for class SystemLinearEquations3D
        case EQUATIONS_NOT_RESULT:  m_error = "SystemLinearEquations3D (number of equations is not equal to number of results)"; break;
        }
    }
    const char* what() const noexcept { return m_error.c_str(); }
};

#endif // EXEPTION_H
