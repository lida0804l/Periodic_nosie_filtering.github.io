#ifndef LIDACOMPLEX_H
#define LIDACOMPLEX_H

#include <iostream>
#include <math.h>

using namespace std;

class LidaComplex
{
public:
    LidaComplex(double real = 0.0, double imaginary = 0.0);

    double real;
    double imaginary;
    double magnitude;
    double phase;

    void set(double real, double imaginary);
    void setPolar(double magnitude, double phase);
//    void multiply_by

private:
};

#endif // LIDACOMPLEX_H
