#ifndef AFFINE_H
#define AFFINE_H

#include <iostream>
#include <QImage>

#include <lidaimage.h>
#include <lidamatrix.h>

using namespace std;

class Affine
{
public:
    Affine();
    enum Interpolation{
        NearestNeighbor,
        Bilinear,
        Bicubic,
    };

    static LidaImage* transformation(LidaImage* input, LidaMatrix* scaling, float scaleFactor = 1, Affine::Interpolation interpolation = Affine::NearestNeighbor);
//    static QImage* transformation(QImage* input, LidaMatrix* scaling, float scaleFactor = 1, Affine::Interpolation interpolation = Affine::NearestNeighbor);

private:
//    void nearest
};

#endif // AFFINE_H

// Testing implementation

//    QImage* redo = image;
//    image = second;

//    float c = cos(-3.14/(180/21));
//    float s = sin(-3.14/(180/21));

//    float matrix[3][3] = {{c, s, 0},
//                          {-s, c, 0},
//                          {0, 0, 1}};

//    float matrix[3][3] = {{10, 0, 0},
//                          {0, 10, 0},
//                          {0, 0, 1}};

//    LidaMatrix* mask = new LidaMatrix(3,3);

//    for(int i = 0; i < mask->row; i++){
//        for(int j = 0; j < mask->col; j++){
//            mask->operator()(i,j) = matrix[i][j];
//        }
//    }

//    LidaImage* input = LIDA::QImage2LidaImage(image);

//    LidaImage* output = Affine::transformation(input, mask, 10, Affine::NearestNeighbor);
//    LidaImage* output = Affine::transformation(input, mask, 10, Affine::Bilinear);
//    LidaImage* output = Affine::transformation(input, mask, 10, Affine::Bicubic);

//    QImage* result = LIDA::LidaImage2QImage(output);

//    delete redo;

//    LIDA::showImage(result);
//    QString savepath = QFileDialog::getSaveFileName(this, tr("Save File"), NULL, tr("jpg (*.jpg);; bmp (*.bmp);; png (*.png)"));
//    second->save(savepath);
