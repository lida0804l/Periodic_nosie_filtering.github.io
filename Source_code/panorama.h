#ifndef PANORAMA_H
#define PANORAMA_H

#include <iostream>
#include <lidaimage.h>
#include <intensitytransformation.h>
#include <spatialfiltering.h>
#include <vector>
#include <QImage>
#include <QPainter>

using namespace std;

class Panorama
{
public:
    Panorama();
    class Descriptor{
    public:
        Descriptor(){x = 0; y = 0; for (int i = 0; i < 128; i++) {hist[i] = 0;}}
        int x;
        int y;
        float hist[128];
    };
    class Match_pair{
    public:
        std::vector<Descriptor> left;
        std::vector<Descriptor> right;
    };

    static LidaImage* warping(LidaImage* input, float focal_length);
    static LidaImage* harris_corner(LidaImage* input, std::vector<Descriptor>& features);

    static Match_pair comparison(std::vector<Descriptor>& features_1, std::vector<Descriptor>& features_2);
    static void outlier_removal(LidaImage* left_image, LidaImage* right_image, Match_pair& match_pair);
    static QImage* draw_feature_match(LidaImage* input1, LidaImage* input2, Match_pair match_pair);
    static LidaImage* auto_stitch();
    static LidaImage* stitch();

private:
    static LidaImage* QImage2LidaImage(QImage* input);
    static QImage* LidaImage2QImage(LidaImage* input);
};

#endif // PANORAMA_H

//    LidaImage* output = Panorama::stitch();
//    QImage* result = LIDA::LidaImage2QImage(output);
//    LIDA::showImage(result);

//    QString savepath = QFileDialog::getSaveFileName(this, tr("Save File"), NULL, tr("jpg (*.jpg);; bmp (*.bmp);; png (*.png)"));
//    result->save(savepath);




//QString fileName = QFileDialog::getOpenFileName(this, "Load Image", 0, "Image Files(*.jpg *.bmp *.png *tif)");
//QImage *image1 = new QImage();
//image1->load(fileName);
//*image1 = image1->convertToFormat(QImage::Format_RGB888);
//LidaImage* input1 = LIDA::QImage2LidaImage(image1);

//QString fileName2 = QFileDialog::getOpenFileName(this, "Load Image", 0, "Image Files(*.jpg *.bmp *.png *tif)");
//QImage *image2 = new QImage();
//image2->load(fileName2);
//*image2 = image2->convertToFormat(QImage::Format_RGB888);
//LidaImage* input2 = LIDA::QImage2LidaImage(image2);

//std::vector<Panorama::Descriptor> features_1;
//std::vector<Panorama::Descriptor> features_2;
//Panorama::harris_corner(input1, features_1);
//Panorama::harris_corner(input2, features_2);

//Panorama::Match_pair match_pair = Panorama::comparison(features_1, features_2);
//Panorama::outlier_removal(input1, input2, match_pair);
//QImage* result = Panorama::draw_feature_match(input1, input2, match_pair);
//cout << match_pair.left.size() << endl;
//LIDA::showImage(result);
