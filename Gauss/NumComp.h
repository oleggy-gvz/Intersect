#ifndef NUMBER_COMPARSION_H
#define NUMBER_COMPARSION_H

bool equal_real(double a, double b)
{
    double eps = 0.0000001;
    double abs = a < b ? b - a : a - b;
    return abs < eps ? true : false;
}

bool less_or_equal_real(double a, double b) { return equal_real(a, b) || a < b; }
bool more_or_equal_real(double a, double b) { return equal_real(a, b) || a > b; }

#endif // NUMBER_COMPARSION_H
