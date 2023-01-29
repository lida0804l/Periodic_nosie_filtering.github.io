#ifndef HDR_H
#define HDR_H

#include <iostream>
#include <lidaimage.h>
#include <lidamatrix.h>
#include <vector>
#include <QImage>
#include <QFileDialog>
#include <fstream>
#include <sstream>

using namespace std;

class Hdr
{
public:
    Hdr();

    static LidaImage* create();

private:
    static std::vector<float> g_solve(std::vector<std::vector<int>> Z, std::vector<float> B, float l, float w[]);

    static LidaImage* QImage2LidaImage(QImage* input);
    static QImage* LidaImage2QImage(LidaImage* input);
};

#endif // HDR_H

//LidaImage* input = LIDA::QImage2LidaImage(image);
//LidaImage* output = Hdr::create();
//output->scaling(LidaImage::full_range);
//scene->lidaimage = output;
//QImage* result = LIDA::LidaImage2QImage(output);
//LIDA::showImage(result);

//QString savepath = QFileDialog::getSaveFileName(this, tr("Save File"), NULL, tr("jpg (*.jpg);; bmp (*.bmp);; png (*.png)"));
//result->save(savepath);
