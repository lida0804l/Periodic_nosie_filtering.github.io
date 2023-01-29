#ifndef LIDA_H
#define LIDA_H

#include <lidascene.h>
#include <lidamatrix.h>
#include <lidaimage.h>
#include <affine.h>
#include <intensitytransformation.h>
#include <spatialfiltering.h>
#include <fouriertransform.h>
#include <panorama.h>
#include <hdr.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QInputDialog>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <time.h>

using namespace std;

//#include <QChart>

namespace Ui {
class LIDA;
}

class LIDA : public QMainWindow
{
    Q_OBJECT

    class descriptor{
    public:
        int x;
        int y;
        float hist[128];
    };

public:
    explicit LIDA(QWidget *parent = nullptr);
    ~LIDA();

//    LidaMatrix<int> a;

private slots:
    void on_actionload_triggered();

    static LidaImage* QImage2LidaImage(QImage* input);
    static QImage* LidaImage2QImage(LidaImage* input);

    void on_actionScaling_triggered();

    void on_actionRotation_triggered();

    void on_actionTranslation_triggered();

    void on_actionShear_vertical_triggered();

    void on_actionShear_horizontal_triggered();

    void on_actionsave_as_triggered();

    void on_actionHistogram_equalization_triggered();

    void on_actionHistogram_specification_triggered();

    void on_actionFuzzy_enhancement_triggered();

    void on_actionGaussian_triggered();

    void on_actionLaplacian_triggered();

    void on_actionLaplacian_sharpening_triggered();

    void on_actionHighboost_sharpening_triggered();

    void on_actionSobel_triggered();

    void on_actionFuzzy_edge_triggered();

    void on_actionILPF_triggered();

    void on_actionBLPF_triggered();

    void on_actionGLPF_triggered();

    void on_actionIHPF_triggered();

    void on_actionBHPF_triggered();

    void on_actionGHPF_triggered();

    void on_actionLaplacian_sharpening_fourier_triggered();

    void on_actionHigh_frequency_emphasis_triggered();

    void on_actionHomomorphic_triggered();

    void on_actionSelective_filtering_triggered();

    void on_actionload_Hdr_triggered();

    void on_actionload_Panorama_triggered();

    void on_actionTest_triggered();

    void on_actionTest2_triggered();

    void on_actionTest3_triggered();

private:
    Ui::LIDA *ui;

//    QImage* image;
    LidaImage* input;

    LidaScene* scene;
    QGraphicsPixmapItem* pixmap_item;

    FourierTransform* freq;

    void showImage(QImage* imageDisplay);
};

#endif // LIDA_H
