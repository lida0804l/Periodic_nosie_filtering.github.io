#ifndef SPATIALFILTERING_H
#define SPATIALFILTERING_H

#include <iostream>
#include <math.h>
#include <QImage>

#include <lidaimage.h>
#include <lidamatrix.h>

using namespace std;

class SpatialFiltering
{
public:
    SpatialFiltering();

    static LidaImage* convolution(LidaImage* input, LidaMatrix* mask);
    static LidaImage* non_maximum_supression(LidaImage* input);

    static LidaImage* average(LidaImage*input, int size = 3);
    static LidaImage* gaussian(LidaImage* input);
    static LidaImage* laplacian(LidaImage* input);
    static LidaImage* laplacianSharpening(LidaImage* input);
    static LidaImage* highboostSharpening(LidaImage* input, int k = 1);
    static LidaImage* sobel(LidaImage* input);
    static LidaImage* gradient_magnitude(LidaImage* input); //For grayscale image
    static LidaImage* gradient_angle(LidaImage* input);// For grayscale image

    static LidaImage* fuzzy(LidaImage* input);
    static LidaImage* edgeDetection(LidaImage* input);
};

#endif // SPATIALFILTERING_H

//    LidaImage* input = LIDA::QImage2LidaImage(image);
//    LidaImage* output = SpatialFiltering::laplacian(input);
//    LidaImage* output = SpatialFiltering::laplacianSharpening(input);
//    LidaImage* output = SpatialFiltering::highboostSharpening(input, 4.5);
//    LidaImage* output = SpatialFiltering::sobel(input);
//    LidaImage* output = SpatialFiltering::fuzzy(input);

//    output->scaling(LidaImage::cutoff);
//    output->scaling(LidaImage::full_range);

//    QImage* result = LIDA::LidaImage2QImage(output);
//    scene->image = result;
//    LIDA::showImage(result);
