#include "affine.h"

Affine::Affine()
{

}

LidaImage* Affine::transformation(LidaImage *input, LidaMatrix* scaling, float scaleFactor, Affine::Interpolation interpolation)
{
    LidaMatrix a(3,3);
    LidaMatrix inverse = scaling->inverse();
    LidaMatrix coordinate(1,3);                //Inverse mapping for pixel in original image

    LidaImage* output = new LidaImage(scaleFactor*(input->row), scaleFactor*(input->col));

    switch (interpolation) {
    case Affine::NearestNeighbor: {
        for (int x = 0; x < output->row; x++) {
            for (int y = 0; y < output->col; y++) {
                coordinate(0,0) = x;
                coordinate(0,1) = y;
                coordinate(0,2) = 1;
                coordinate = coordinate*inverse;

                if(((coordinate(0,0) >= 0) && ((coordinate(0,0) < input->row))) && ((coordinate(0,1) >= 0) && ((coordinate(0,1) < input->col)))){
                    output->matrix3D[0][x][y] = input->matrix3D[0][int(coordinate(0,0))][int(coordinate(0,1))];
                    output->matrix3D[1][x][y] = input->matrix3D[1][int(coordinate(0,0))][int(coordinate(0,1))];
                    output->matrix3D[2][x][y] = input->matrix3D[2][int(coordinate(0,0))][int(coordinate(0,1))];
                }
            }
        }
        break;
    }
    case Affine::Bilinear: {
        LidaMatrix X(4,4);                                                  //location = { 0, 0
        int tmp[4][2];                                                      //             0, 1
        int index = 0;                                                      //             1, 0
                                                                            //             1, 1 }
        for (int x = 0; x <= 1; x++) {                                      //
            for (int y = 0; y <= 1; y++) {                                  //
                tmp[index][0] = x;                                          //
                tmp[index][1] = y;                                          //
                index++;                                                    //
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        //
        }                                                                   //

        for (int idx = 0; idx < X.row; idx++) {                             //X = { 1 0 0 0
            for (int i = 0; i <= 1; i++) {                                  //      1 1 0 0
                for (int j = 0; j <= 1; j++) {                              //      1 0 1 0
                    X(idx, 2*i+j) = pow(tmp[idx][0],i)*pow(tmp[idx][1],j);  //      1 1 1 1 }
                }
            }
        }

        ////    X.displayMatrix2D();

        LidaMatrix r(4,1);   //r = Xc
        LidaMatrix g(4,1);   //g = Xc
        LidaMatrix b(4,1);   //b = Xc

        LidaMatrix c(4,1);
        LidaMatrix XInverse = X.inverse();

        int rgb[3];

        for (int x = 0; x < output->row; x++) {
            for (int y = 0; y < output->col; y++) {
                coordinate(0,0) = x;
                coordinate(0,1) = y;
                coordinate(0,2) = 1;
                coordinate = coordinate*inverse;

                if(((coordinate(0,0) >= 0) && ((coordinate(0,0) < (input->row-1)))) && ((coordinate(0,1) >= 0) && ((coordinate(0,1) < (input->col-1))))){
                    //output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
                    int count = 0;
                    for (int x = int(coordinate(0,0)); x <= (int(coordinate(0,0))+1); x++) {
                        for (int y = int(coordinate(0,1)); y <= (int(coordinate(0,1))+1); y++) {
                            r(count,0) = input->matrix3D[0][x][y];
                            g(count,0) = input->matrix3D[1][x][y];
                            b(count,0) = input->matrix3D[2][x][y];
                            count++;
                        }
                    }
                    count = 0;

                    float fraction_X = coordinate(0,0) - (int(coordinate(0,0)));
                    float fraction_Y = coordinate(0,1) - (int(coordinate(0,1)));

                    c = XInverse*r;
                    float tmp = 0;
                    for (int i = 0; i <=1; i++) {
                        for (int j = 0; j <= 1; j++) {
                            tmp += c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[0] = tmp;
                    tmp = 0;

                    c = XInverse*g;
                    for (int i = 0; i <=1; i++) {
                        for (int j = 0; j <= 1; j++) {
                            tmp += c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[1] = tmp;
                    tmp = 0;

                    c = XInverse*b;
                    for (int i = 0; i <=1; i++) {
                        for (int j = 0; j <= 1; j++) {
                            tmp += (c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j));
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[2] = tmp;
                    tmp = 0;

                    output->matrix3D[0][x][y] = rgb[0];
                    output->matrix3D[1][x][y] = rgb[1];
                    output->matrix3D[2][x][y] = rgb[2];
                }
            }
        }
        break;
    }
    case Affine::Bicubic: {
        LidaMatrix X(16,16);                                                //location = { -1, -1
        int tmp[16][2];                                                     //               to
        int index = 0;                                                      //              2, 2 }
                                                                            //
        for (int x = -1; x <= 2; x++) {                                     //
            for (int y = -1; y <= 2; y++) {                                 //
                tmp[index][0] = x;                                          //
                tmp[index][1] = y;                                          //
                index++;                                                    //
            }                                                               //
        }                                                                   //

        for (int idx = 0; idx < X.row; idx++) {                             //X = {
            for (int i = 0; i <= 3; i++) {                                  //
                for (int j = 0; j <= 3; j++) {                              //
                    X(idx, 4*i+j) = pow(tmp[idx][0],i)*pow(tmp[idx][1],j);  //      }
                }
            }
        }

        LidaMatrix r(16,1);   //r = Xc
        LidaMatrix g(16,1);   //g = Xc
        LidaMatrix b(16,1);   //b = Xc

        LidaMatrix c(16,1);
        LidaMatrix XInverse = X.inverse();

        int rgb[3];

        for (int x = 0; x < output->row; x++) {
            for (int y = 0; y < output->col; y++) {
                //    for (int x = 158; x < 159; x++) {
                //        for (int y = 152; y < 153; y++) {
                coordinate(0,0) = x;
                coordinate(0,1) = y;
                coordinate(0,2) = 1;
                coordinate = coordinate*inverse;

                if(((coordinate(0,0) >= 1) && ((coordinate(0,0) < (input->row-2)))) && ((coordinate(0,1) >= 1) && ((coordinate(0,1) < (input->col-2))))){
                    float fraction_X = coordinate(0,0) - (int(coordinate(0,0)));
                    float fraction_Y = coordinate(0,1) - (int(coordinate(0,1)));
//                    if((fraction_X < 0.00000000000005) && (fraction_Y < 0.00000000000005)){
//                        output->matrix3D[0][x][y] =
//                        output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
//                        continue;
//                    }
                    int count = 0;
                    for (int i = int(coordinate(0,0)-1); i <= (int(coordinate(0,0))+2); i++) {
                        for (int j = int(coordinate(0,1)-1); j <= (int(coordinate(0,1))+2); j++) {
                            //                        cout << x << " " << y << endl;
                            r(count,0) = input->matrix3D[0][i][j];
                            g(count,0) = input->matrix3D[1][i][j];
                            b(count,0) = input->matrix3D[2][i][j];
                            count++;
                        }
                    }
                    count = 0;

                    c = XInverse*r;
                    float tmp = 0;
                    for (int i = 0; i <=3; i++) {
                        for (int j = 0; j <= 3; j++) {
//                            cout << i << " " << j << " " << pow(fraction_X,i) << " " << pow(fraction_Y,j) << endl;
                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[0] = tmp;
                    tmp = 0;

                    c = XInverse*g;
                    for (int i = 0; i <=3; i++) {
                        for (int j = 0; j <= 3; j++) {
                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[1] = tmp;
                    tmp = 0;

                    c = XInverse*b;
                    for (int i = 0; i <=3; i++) {
                        for (int j = 0; j <= 3; j++) {
                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
                        }
                    }
                    if(tmp > 255){
                        tmp = 255;
                    }
                    else if (tmp < 0) {
                        tmp = 0;
                    }
                    rgb[2] = tmp;
                    tmp = 0;

                    output->matrix3D[0][x][y] = rgb[0];
                    output->matrix3D[1][x][y] = rgb[1];
                    output->matrix3D[2][x][y] = rgb[2];
                }
            }
        }
        break;
    }
//    default:
//        break;
    }
    return output;
}







//QImage* Affine::transformation(QImage* input, LidaMatrix* scaling, float scaleFactor, Affine::Interpolation interpolation)
//{
//    LidaMatrix a(3,3);
//    LidaMatrix inverse = scaling->inverse();
//    LidaMatrix coordinate(1,3);                //Inverse mapping for pixel in original image

//    QImage* output = new QImage(scaleFactor*(input->width()), scaleFactor*(input->height()), input->format());

//    switch (interpolation) {
//    case Affine::NearestNeighbor: {
//        for (int x = 0; x < output->height(); x++) {
//            for (int y = 0; y < output->width(); y++) {
//                coordinate(0,0) = x;
//                coordinate(0,1) = y;
//                coordinate(0,2) = 1;
//                coordinate = coordinate*inverse;

//                if(((coordinate(0,0) >= 0) && ((coordinate(0,0) < input->height()))) && ((coordinate(0,1) >= 0) && ((coordinate(0,1) < input->width())))){
//                    output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
//                }
//                //            cout << "x: " << x << " "
//                //                 << "y: " << y << " " << endl;
//                //            cout << "r: " << image->pixelColor(y,x).red() << " "
//                //                 << "g: " << image->pixelColor(y,x).green() << " "
//                //                 << "b: " << image->pixelColor(y,x).blue() << endl;
//            }
//        }
//        break;
//    }
//    case Affine::Bilinear: {
//        LidaMatrix X(4,4);                                                  //location = { 0, 0
//        int tmp[4][2];                                                      //             0, 1
//        int index = 0;                                                      //             1, 0
//                                                                            //             1, 1 }
//        for (int x = 0; x <= 1; x++) {                                      //
//            for (int y = 0; y <= 1; y++) {                                  //
//                tmp[index][0] = x;                                          //
//                tmp[index][1] = y;                                          //
//                index++;                                                    //
//            }                                                               //
//        }                                                                   //

//        for (int idx = 0; idx < X.row; idx++) {                             //X = { 1 0 0 0
//            for (int i = 0; i <= 1; i++) {                                  //      1 1 0 0
//                for (int j = 0; j <= 1; j++) {                              //      1 0 1 0
//                    X(idx, 2*i+j) = pow(tmp[idx][0],i)*pow(tmp[idx][1],j);  //      1 1 1 1 }
//                }
//            }
//        }

//        ////    X.displayMatrix2D();

//        LidaMatrix r(4,1);   //r = Xc
//        LidaMatrix g(4,1);   //g = Xc
//        LidaMatrix b(4,1);   //b = Xc

//        LidaMatrix c(4,1);
//        LidaMatrix XInverse = X.inverse();

//        int rgb[3];

//        for (int x = 0; x < output->height(); x++) {
//            for (int y = 0; y < output->width(); y++) {
//                coordinate(0,0) = x;
//                coordinate(0,1) = y;
//                coordinate(0,2) = 1;
//                coordinate = coordinate*inverse;

//                if(((coordinate(0,0) >= 0) && ((coordinate(0,0) < (input->height()-1)))) && ((coordinate(0,1) >= 0) && ((coordinate(0,1) < (input->width()-1))))){
//                    //output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
//                    int count = 0;
//                    for (int x = int(coordinate(0,0)); x <= (int(coordinate(0,0))+1); x++) {
//                        for (int y = int(coordinate(0,1)); y <= (int(coordinate(0,1))+1); y++) {
//                            QColor color = input->pixelColor(y,x);

//                            r(count,0) = color.red();
//                            g(count,0) = color.green();
//                            b(count,0) = color.blue();
//                            count++;
//                        }
//                    }
//                    count = 0;

//                    float fraction_X = coordinate(0,0) - (int(coordinate(0,0)));
//                    float fraction_Y = coordinate(0,1) - (int(coordinate(0,1)));

//                    c = XInverse*r;
//                    float tmp = 0;
//                    for (int i = 0; i <=1; i++) {
//                        for (int j = 0; j <= 1; j++) {
//                            tmp += c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[0] = tmp;
//                    tmp = 0;

//                    c = XInverse*g;
//                    for (int i = 0; i <=1; i++) {
//                        for (int j = 0; j <= 1; j++) {
//                            tmp += c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[1] = tmp;
//                    tmp = 0;

//                    c = XInverse*b;
//                    for (int i = 0; i <=1; i++) {
//                        for (int j = 0; j <= 1; j++) {
//                            tmp += (c(2*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j));
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[2] = tmp;
//                    tmp = 0;

//                    output->setPixel(y,x,qRgb(rgb[0],rgb[1],rgb[2]));
//                }
//            }
//        }
//        break;
//    }
//    case Affine::Bicubic: {
//        LidaMatrix X(16,16);                                                //location = { -1, -1
//        int tmp[16][2];                                                     //               to
//        int index = 0;                                                      //              2, 2 }
//                                                                            //
//        for (int x = -1; x <= 2; x++) {                                     //
//            for (int y = -1; y <= 2; y++) {                                 //
//                tmp[index][0] = x;                                          //
//                tmp[index][1] = y;                                          //
//                index++;                                                    //
//            }                                                               //
//        }                                                                   //

//        for (int idx = 0; idx < X.row; idx++) {                             //X = {
//            for (int i = 0; i <= 3; i++) {                                  //
//                for (int j = 0; j <= 3; j++) {                              //
//                    X(idx, 4*i+j) = pow(tmp[idx][0],i)*pow(tmp[idx][1],j);  //      }
//                }
//            }
//        }

//        LidaMatrix r(16,1);   //r = Xc
//        LidaMatrix g(16,1);   //g = Xc
//        LidaMatrix b(16,1);   //b = Xc

//        LidaMatrix c(16,1);
//        LidaMatrix XInverse = X.inverse();

//        int rgb[3];

//        for (int x = 0; x < output->height(); x++) {
//            for (int y = 0; y < output->width(); y++) {
//                //    for (int x = 158; x < 159; x++) {
//                //        for (int y = 152; y < 153; y++) {
//                coordinate(0,0) = x;
//                coordinate(0,1) = y;
//                coordinate(0,2) = 1;
//                coordinate = coordinate*inverse;

//                if(((coordinate(0,0) >= 1) && ((coordinate(0,0) < (input->height()-2)))) && ((coordinate(0,1) >= 1) && ((coordinate(0,1) < (input->width()-2))))){
//                    float fraction_X = coordinate(0,0) - (int(coordinate(0,0)));
//                    float fraction_Y = coordinate(0,1) - (int(coordinate(0,1)));
//                    if((fraction_X < 0.00000000000005) && (fraction_Y < 0.00000000000005)){
//                        output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
//                        continue;
//                    }
//                    int count = 0;
//                    for (int i = int(coordinate(0,0)-1); i <= (int(coordinate(0,0))+2); i++) {
//                        for (int j = int(coordinate(0,1)-1); j <= (int(coordinate(0,1))+2); j++) {
//                            //                        cout << x << " " << y << endl;
//                            QColor color = input->pixelColor(j,i);
//                            r(count,0) = color.red();
//                            g(count,0) = color.green();
//                            b(count,0) = color.blue();
//                            count++;
//                        }
//                    }
//                    count = 0;

//                    //                float fraction_X = coordinate(0,0) - (int(coordinate(0,0)));
//                    //                float fraction_Y = coordinate(0,1) - (int(coordinate(0,1)));

//                    c = XInverse*r;
//                    float tmp = 0;
//                    for (int i = 0; i <=3; i++) {
//                        for (int j = 0; j <= 3; j++) {
//                            //                        cout << i << " " << j << " " << pow(fraction_X,i) << " " << pow(fraction_Y,j) << endl;
//                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[0] = tmp;
//                    tmp = 0;

//                    c = XInverse*g;
//                    for (int i = 0; i <=3; i++) {
//                        for (int j = 0; j <= 3; j++) {
//                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[1] = tmp;
//                    tmp = 0;

//                    c = XInverse*b;
//                    for (int i = 0; i <=3; i++) {
//                        for (int j = 0; j <= 3; j++) {
//                            tmp += c(4*i+j,0)*pow(fraction_X,i)*pow(fraction_Y,j);
//                        }
//                    }
//                    if(tmp > 255){
//                        tmp = 255;
//                    }
//                    else if (tmp < 0) {
//                        tmp = 0;
//                    }
//                    rgb[2] = tmp;
//                    tmp = 0;

//                    ////            cout << x << " " << y << " " << rgb[0] << " " << rgb[1] << " " << rgb[2] << endl;
//                    output->setPixel(y,x,qRgb(rgb[0],rgb[1],rgb[2]));
//                }
////                else {
////                    output->setPixel(y,x,input->pixelColor(coordinate(0,1),coordinate(0,0)).rgb());
////                }
//            }
//        }
//        break;
//    }
////    default:
////        break;
//    }
//    return output;
//}
