#ifndef LIDAMATRIX_H
#define LIDAMATRIX_H

#include <iostream>
#include <math.h>

using namespace std;

class LidaMatrix
{
public:
    LidaMatrix(int row = 0, int col = 0);
    ~LidaMatrix();

    int row;
    int col;

    float** matrix2D;

    void displayMatrix2D();
    float determinant();

    void operator=(const LidaMatrix& matrix);
    float& operator()(int i, int j);
    LidaMatrix operator*(LidaMatrix& matrix);
    LidaMatrix transpose();
    LidaMatrix inverse();

private:   
//    float** matrix2D;
    LidaMatrix C(int i, int j, LidaMatrix* matrix);

    void addRow(int rowi, float coefficient, int rowj);
    void swapRow(int rowi, int rowj);
    void scale(int rowj, float coefficient);
};

#endif // LIDAMATRIX_H
