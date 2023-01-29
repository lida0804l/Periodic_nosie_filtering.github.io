#ifndef INTENSITYTRANSFORMATION_H
#define INTENSITYTRANSFORMATION_H

#include <iostream>
#include <QImage>
#include <QFileDialog>
#include <fstream>
#include <sstream>

#include <lidaimage.h>

using namespace std;

class IntensityTransformation
{
public:
    IntensityTransformation();

    static LidaImage* histogram_equalization(LidaImage* input);
    static LidaImage* histogram_specification(LidaImage* input);

//    static LidaImage* grayscale(LidaImage* input);
    static LidaImage* fuzzy(LidaImage* input);
};

#endif // INTENSITYTRANSFORMATION_H

//    LidaImage* input = LIDA::QImage2LidaImage(image);
//    LidaImage* output = IntensityTransformation::histogram_equalization(input);
//    LidaImage* output = IntensityTransformation::histogram_specification(input);
//    LidaImage* output = IntensityTransformation::fuzzy(input);

//    output->scaling(LidaImage::cutoff);
//    QImage* result = LIDA::LidaImage2QImage(output);
//    LIDA::showImage(result);
