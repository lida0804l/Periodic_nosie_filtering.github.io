#include "lidamatrix.h"

LidaMatrix::LidaMatrix(int row, int col)
{
    this->row = row;
    this->col = col;

    this->matrix2D = new float*[row];
    for(int i = 0; i < row; i++){
        this->matrix2D[i] = new float[col];
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            this->matrix2D[i][j] = 0;
        }
    }
}

LidaMatrix::~LidaMatrix()
{
    for(int i = 0; i < row; ++i){
        delete[] matrix2D[i];
    }
    delete[] matrix2D;
    matrix2D = 0;
}

void LidaMatrix::displayMatrix2D()
{
    for(int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            cout << matrix2D[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

float LidaMatrix::determinant()
{
    if(this->row != this->col){
        cout << "Not a square matrix!" << endl;
        return 0;
    }

    if((this->row == 1) && (this->col == 1)){
        return this->matrix2D[0][0];
    }

    float det = 0;
    for(int j = 0; j < col; j++){
        float det_element = this->matrix2D[0][j];

        det_element *= pow(-1,1+(j+1));
//        cout << "current det_element: " << det_element << endl;
        det_element *= C(0,j,this).determinant();

        det += det_element;

//        cout << "current det_element: " << det_element << endl;
//        cout << "current det: " << det << endl << endl;
    }
    return det;
}

void LidaMatrix::operator=(const LidaMatrix& matrix)
{
    if(matrix.matrix2D != NULL){
        for(int i = 0; i < row; i++){
            delete[] matrix2D[i];
        }
        delete[] matrix2D;
        this->matrix2D = 0;
    }

    this->row = matrix.row;
    this->col = matrix.col;

    this->matrix2D = new float*[this->row];
    for(int i = 0; i < this->row; i++){
        this->matrix2D[i] = new float[this->col];
    }

    for(int i = 0; i < this->row; i++){
        for(int j = 0; j < this->col; j++){
            this->matrix2D[i][j] = matrix.matrix2D[i][j];
        }
    }

//    matrix.col = 1;
}

float& LidaMatrix::operator()(int i, int j)
{
    return matrix2D[i][j];
}

LidaMatrix LidaMatrix::operator*(LidaMatrix& matrix)
{
//    cout << this->row << " " << matrix.col;
    LidaMatrix output(this->row, matrix.col);

    if(this->col == matrix.row){
        for (int i = 0; i < output.row; i++) {
            for (int j = 0; j < output.col; j++) {
                float value = 0;
                for (int idx = 0; idx < this->col; idx++) {
                    value += (this->matrix2D[i][idx])*(matrix(idx,j));
                }
                output(i,j) = value;
            }
        }
    }
    else {
        cout << "Matrix multiplication fail" << endl;
    }
    return output;
}

LidaMatrix LidaMatrix::transpose()
{
    LidaMatrix output(this->col, this->row);

    for (int i = 0; i < output.row; i++) {
        for (int j = 0; j < output.col; j++) {
            output(i,j) = this->matrix2D[j][i];
        }
    }

    return output;
}

LidaMatrix LidaMatrix::inverse()
{
    // Cramer's rules
//    LidaMatrix inverse(this->row, this->col);

//    float det = this->determinant();
//    for (int i = 0; i < this->row; i++) {
//        for (int j = 0; j < this->col; j++) {
//            inverse(i,j) = pow(-1,(i+1)+(j+1))*(1/det)*C(j,i,this).determinant();
//        }
//    }

//    return inverse;

    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gauss elimination
    bool invertible = true;
    LidaMatrix inverse(this->row, this->col);
    LidaMatrix clone(this->row, this->col);

    for (int i = 0; i < this->row; i++) {          //Copy a clone matrix for current matrix
        for (int j = 0; j < this->col; j++) {
            clone(i,j) = this->matrix2D[i][j];
        }
    }

    for(int i = 0; i < this->row; i++) {           //Form a Identity matrix as inverse
        inverse(i,i) = 1;
    }

//    clone.displayMatrix2D();


    if(this->row == this->col){
        for (int j = 0; j < this->col; j++) {                      //In a square matrix, (j,j) should be the pivot
            if(abs(clone(j,j)) < 0.000000005){                         //If the value of the pivot is zero, swap that row with a nonzero one
//                cout << "j: " << j << endl;
                for (int i = (j+1); i < this->row; i++) {
                    if(abs(clone(i,j)) > 0.000000005){
//                        cout << "swap" << endl;
                        clone.swapRow(i,j);
                        inverse.swapRow(i,j);
//                        clone.displayMatrix2D();
//                        inverse.displayMatrix2D();
                        break;
                    }
                }
            }
            if(abs(clone(j,j)) < 0.000000005){
                invertible = false;
//                cout << "Find zero on pivot (matrix uninvertible)! " << "( " << j << " , " << j << " )" << endl;
                break;                                             //If the the value is still zero, then this is a non-invertible matrix
            }


            for (int i = (j+1); i < this->row; i++) {              //Do Gauss elimination
                if(abs(clone(i,j)) < 0.000000005){                     //If the current element is zero,
//                    cout << "i: " << i << " j: " << j << " next row element zero" << endl;
                    continue;
                }
//                cout << "i: " << i << " j: " << j << endl;
                float corfficient = (-1)*clone(i,j)/clone(j,j);
                clone.addRow(i, corfficient, j);
                inverse.addRow(i, corfficient, j);
//                clone.displayMatrix2D();
//                inverse.displayMatrix2D();
            }
        }

//        bool invertible = true;
        for (int j = 0; j < this->col; j++) {
            if(abs(clone(j,j)) <= 0.000000005){
                invertible = false;
                cout << "Find zero on pivot! " << "( " << j << " , " << j << " )" << endl;   //need
            }
        }

        if(invertible == true){
            for (int j = 0; j < this->col; j++) {                //If matrix invertible, all pivot scale up to value 1
                inverse.scale(j,1/clone(j,j));
                clone.scale(j,(1/clone(j,j)));
            }

            for(int j = (this->col -1); j >= 0; j--){
                for (int i = (j-1); i >= 0; i--) {
//                    cout << "i: " << i << " j: " << j << endl;
                    float corfficient = (-1)*clone(i,j)/clone(j,j);
                    clone.addRow(i, corfficient, j);
                    inverse.addRow(i, corfficient, j);
//                    clone.displayMatrix2D();
//                    inverse.displayMatrix2D();
                }
            }

        }
        else{
//            this->displayMatrix2D();
            cout << "matrix uninvertible" << endl;     //need
        }
//        clone.displayMatrix2D();
//        inverse.displayMatrix2D();
    }
    return inverse;
}

LidaMatrix LidaMatrix::C(int i, int j, LidaMatrix* matrix)
{
    LidaMatrix reduction(matrix->row-1, matrix->col-1);

    for(int x = 0; x < reduction.row; x++){
        for (int y = 0; y < reduction.col; y++){
            if((x>=i) && (y>=j)){
                reduction(x,y) = matrix->operator()(x+1,y+1);
            }
            else if(x >= i){
                reduction(x,y) = matrix->operator()(x+1,y);
            }
            else if(y >= j){
                reduction(x,y) = matrix->operator()(x,y+1);
            }
            else {
                reduction(x,y) = matrix->operator()(x,y);
            }
        }
    }
//    reduction.displayMatrix2D();
    return reduction;
}

void LidaMatrix::addRow(int rowi, float coefficient, int rowj)
{
    for(int j = 0; j < this->col; j ++){
        this->matrix2D[rowi][j] += coefficient*this->matrix2D[rowj][j];
    }
}

void LidaMatrix::swapRow(int rowi, int rowj)
{
    float temp;
    for(int j = 0; j < this->col; j++){
        temp = this->matrix2D[rowi][j];
        this->matrix2D[rowi][j] = this->matrix2D[rowj][j];
        this->matrix2D[rowj][j] = temp;
    }
}

void LidaMatrix::scale(int rowj, float coefficient)
{
    for(int j = 0; j < this->col; j++){
        this->matrix2D[rowj][j] *= coefficient;
    }
}
