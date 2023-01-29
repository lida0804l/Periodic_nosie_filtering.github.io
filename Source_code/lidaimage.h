#ifndef LIDAIMAGE_H
#define LIDAIMAGE_H

#include <iostream>
#include <math.h>
#include <random>

using namespace std;

class LidaImage
{
public:
    LidaImage(int row = 0, int col = 0);
    ~LidaImage();
    enum Calibration{
        cutoff,
        full_range,
    };

    int row;
    int col;
    float*** matrix3D;

    LidaImage* grayscale();
    LidaImage* RGB2HSI();
    LidaImage* HSI2RGB();
    void scaling(LidaImage::Calibration calibration = LidaImage::cutoff);

    LidaImage* addGaussianNoise(float mean = 0, float std = 10);
    static float PSNR(LidaImage* input, LidaImage* benchmark, int k = 8);
    static float SSIM(LidaImage* input, LidaImage* benchmark, int k = 8);

private:

};

#endif // LIDAIMAGE_H
