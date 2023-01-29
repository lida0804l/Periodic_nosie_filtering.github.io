#include "lidacomplex.h"

LidaComplex::LidaComplex(double real, double imaginary)
{
    LidaComplex::set(real, imaginary);
}

void LidaComplex::set(double real, double imaginary)
{
    this->real = real;
    this->imaginary = imaginary;

    this->magnitude = std::sqrt(pow(this->real,2) + pow(this->imaginary,2));
    this->phase = std::atan2(this->imaginary, this->real);
}

void LidaComplex::setPolar(double magnitude, double phase)
{
    this->magnitude = magnitude;
    this->phase = phase;

    this->real = magnitude * std::cos(phase);
    this->imaginary = magnitude * std::sin(phase);
}
