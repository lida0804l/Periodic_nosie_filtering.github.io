/********************************************************************************
** Form generated from reading UI file 'lida.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIDA_H
#define UI_LIDA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LIDA
{
public:
    QAction *actionload;
    QAction *actionScaling;
    QAction *actionHistogram_equalization;
    QAction *actionGaussian;
    QAction *actionRotation;
    QAction *actionTranslation;
    QAction *actionShear_vertical;
    QAction *actionShear_horizontal;
    QAction *actionsave_as;
    QAction *actionHistogram_specification;
    QAction *actionFuzzy_enhancement;
    QAction *actionLaplacian;
    QAction *actionLaplacian_sharpening;
    QAction *actionHighboost_sharpening;
    QAction *actionSobel;
    QAction *actionFuzzy_edge;
    QAction *actionILPF;
    QAction *actionBLPF;
    QAction *actionGLPF;
    QAction *actionIHPF;
    QAction *actionBHPF;
    QAction *actionGHPF;
    QAction *actionLaplacian_sharpening_fourier;
    QAction *actionHigh_frequency_emphasis;
    QAction *actionHomomorphic;
    QAction *actionSelective_filtering;
    QAction *actionload_Hdr;
    QAction *actionload_Panorama;
    QAction *actionTest;
    QAction *actionTest2;
    QAction *actionTest3;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menutest;
    QMenu *menutest3;
    QMenu *menuFourier_transform;
    QMenu *menuAbout_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LIDA)
    {
        if (LIDA->objectName().isEmpty())
            LIDA->setObjectName(QString::fromUtf8("LIDA"));
        LIDA->resize(1120, 683);
        actionload = new QAction(LIDA);
        actionload->setObjectName(QString::fromUtf8("actionload"));
        actionScaling = new QAction(LIDA);
        actionScaling->setObjectName(QString::fromUtf8("actionScaling"));
        actionHistogram_equalization = new QAction(LIDA);
        actionHistogram_equalization->setObjectName(QString::fromUtf8("actionHistogram_equalization"));
        actionGaussian = new QAction(LIDA);
        actionGaussian->setObjectName(QString::fromUtf8("actionGaussian"));
        actionRotation = new QAction(LIDA);
        actionRotation->setObjectName(QString::fromUtf8("actionRotation"));
        actionTranslation = new QAction(LIDA);
        actionTranslation->setObjectName(QString::fromUtf8("actionTranslation"));
        actionShear_vertical = new QAction(LIDA);
        actionShear_vertical->setObjectName(QString::fromUtf8("actionShear_vertical"));
        actionShear_horizontal = new QAction(LIDA);
        actionShear_horizontal->setObjectName(QString::fromUtf8("actionShear_horizontal"));
        actionsave_as = new QAction(LIDA);
        actionsave_as->setObjectName(QString::fromUtf8("actionsave_as"));
        actionHistogram_specification = new QAction(LIDA);
        actionHistogram_specification->setObjectName(QString::fromUtf8("actionHistogram_specification"));
        actionFuzzy_enhancement = new QAction(LIDA);
        actionFuzzy_enhancement->setObjectName(QString::fromUtf8("actionFuzzy_enhancement"));
        actionLaplacian = new QAction(LIDA);
        actionLaplacian->setObjectName(QString::fromUtf8("actionLaplacian"));
        actionLaplacian_sharpening = new QAction(LIDA);
        actionLaplacian_sharpening->setObjectName(QString::fromUtf8("actionLaplacian_sharpening"));
        actionHighboost_sharpening = new QAction(LIDA);
        actionHighboost_sharpening->setObjectName(QString::fromUtf8("actionHighboost_sharpening"));
        actionSobel = new QAction(LIDA);
        actionSobel->setObjectName(QString::fromUtf8("actionSobel"));
        actionFuzzy_edge = new QAction(LIDA);
        actionFuzzy_edge->setObjectName(QString::fromUtf8("actionFuzzy_edge"));
        actionILPF = new QAction(LIDA);
        actionILPF->setObjectName(QString::fromUtf8("actionILPF"));
        actionBLPF = new QAction(LIDA);
        actionBLPF->setObjectName(QString::fromUtf8("actionBLPF"));
        actionGLPF = new QAction(LIDA);
        actionGLPF->setObjectName(QString::fromUtf8("actionGLPF"));
        actionIHPF = new QAction(LIDA);
        actionIHPF->setObjectName(QString::fromUtf8("actionIHPF"));
        actionBHPF = new QAction(LIDA);
        actionBHPF->setObjectName(QString::fromUtf8("actionBHPF"));
        actionGHPF = new QAction(LIDA);
        actionGHPF->setObjectName(QString::fromUtf8("actionGHPF"));
        actionLaplacian_sharpening_fourier = new QAction(LIDA);
        actionLaplacian_sharpening_fourier->setObjectName(QString::fromUtf8("actionLaplacian_sharpening_fourier"));
        actionHigh_frequency_emphasis = new QAction(LIDA);
        actionHigh_frequency_emphasis->setObjectName(QString::fromUtf8("actionHigh_frequency_emphasis"));
        actionHomomorphic = new QAction(LIDA);
        actionHomomorphic->setObjectName(QString::fromUtf8("actionHomomorphic"));
        actionSelective_filtering = new QAction(LIDA);
        actionSelective_filtering->setObjectName(QString::fromUtf8("actionSelective_filtering"));
        actionload_Hdr = new QAction(LIDA);
        actionload_Hdr->setObjectName(QString::fromUtf8("actionload_Hdr"));
        actionload_Panorama = new QAction(LIDA);
        actionload_Panorama->setObjectName(QString::fromUtf8("actionload_Panorama"));
        actionTest = new QAction(LIDA);
        actionTest->setObjectName(QString::fromUtf8("actionTest"));
        actionTest2 = new QAction(LIDA);
        actionTest2->setObjectName(QString::fromUtf8("actionTest2"));
        actionTest3 = new QAction(LIDA);
        actionTest3->setObjectName(QString::fromUtf8("actionTest3"));
        centralWidget = new QWidget(LIDA);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        LIDA->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LIDA);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1120, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menutest = new QMenu(menuBar);
        menutest->setObjectName(QString::fromUtf8("menutest"));
        menutest3 = new QMenu(menuBar);
        menutest3->setObjectName(QString::fromUtf8("menutest3"));
        menuFourier_transform = new QMenu(menuBar);
        menuFourier_transform->setObjectName(QString::fromUtf8("menuFourier_transform"));
        menuAbout_2 = new QMenu(menuBar);
        menuAbout_2->setObjectName(QString::fromUtf8("menuAbout_2"));
        LIDA->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LIDA);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        LIDA->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LIDA);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        LIDA->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuBar->addAction(menutest->menuAction());
        menuBar->addAction(menutest3->menuAction());
        menuBar->addAction(menuFourier_transform->menuAction());
        menuBar->addAction(menuAbout_2->menuAction());
        menuFile->addAction(actionload);
        menuFile->addAction(actionload_Hdr);
        menuFile->addAction(actionload_Panorama);
        menuFile->addAction(actionsave_as);
        menuAbout->addAction(actionScaling);
        menuAbout->addAction(actionRotation);
        menuAbout->addAction(actionTranslation);
        menuAbout->addAction(actionShear_vertical);
        menuAbout->addAction(actionShear_horizontal);
        menutest->addAction(actionHistogram_equalization);
        menutest->addAction(actionHistogram_specification);
        menutest->addAction(actionFuzzy_enhancement);
        menutest3->addAction(actionGaussian);
        menutest3->addAction(actionLaplacian);
        menutest3->addAction(actionLaplacian_sharpening);
        menutest3->addAction(actionHighboost_sharpening);
        menutest3->addAction(actionSobel);
        menutest3->addAction(actionFuzzy_edge);
        menuFourier_transform->addAction(actionILPF);
        menuFourier_transform->addAction(actionBLPF);
        menuFourier_transform->addAction(actionGLPF);
        menuFourier_transform->addAction(actionIHPF);
        menuFourier_transform->addAction(actionBHPF);
        menuFourier_transform->addAction(actionGHPF);
        menuFourier_transform->addAction(actionLaplacian_sharpening_fourier);
        menuFourier_transform->addAction(actionHigh_frequency_emphasis);
        menuFourier_transform->addAction(actionHomomorphic);
        menuFourier_transform->addAction(actionSelective_filtering);
        menuAbout_2->addAction(actionTest);
        menuAbout_2->addAction(actionTest2);
        menuAbout_2->addAction(actionTest3);

        retranslateUi(LIDA);

        QMetaObject::connectSlotsByName(LIDA);
    } // setupUi

    void retranslateUi(QMainWindow *LIDA)
    {
        LIDA->setWindowTitle(QApplication::translate("LIDA", "LIDA", nullptr));
        actionload->setText(QApplication::translate("LIDA", "load", nullptr));
        actionScaling->setText(QApplication::translate("LIDA", "Scaling", nullptr));
        actionHistogram_equalization->setText(QApplication::translate("LIDA", "Histogram_equalization", nullptr));
        actionGaussian->setText(QApplication::translate("LIDA", "Gaussian", nullptr));
        actionRotation->setText(QApplication::translate("LIDA", "Rotation", nullptr));
        actionTranslation->setText(QApplication::translate("LIDA", "Translation", nullptr));
        actionShear_vertical->setText(QApplication::translate("LIDA", "Shear_vertical", nullptr));
        actionShear_horizontal->setText(QApplication::translate("LIDA", "Shear_horizontal", nullptr));
        actionsave_as->setText(QApplication::translate("LIDA", "save as", nullptr));
        actionHistogram_specification->setText(QApplication::translate("LIDA", "Histogram_specification", nullptr));
        actionFuzzy_enhancement->setText(QApplication::translate("LIDA", "Fuzzy_enhancement", nullptr));
        actionLaplacian->setText(QApplication::translate("LIDA", "Laplacian", nullptr));
        actionLaplacian_sharpening->setText(QApplication::translate("LIDA", "Laplacian_sharpening", nullptr));
        actionHighboost_sharpening->setText(QApplication::translate("LIDA", "Highboost_sharpening", nullptr));
        actionSobel->setText(QApplication::translate("LIDA", "Sobel", nullptr));
        actionFuzzy_edge->setText(QApplication::translate("LIDA", "Fuzzy_edge", nullptr));
        actionILPF->setText(QApplication::translate("LIDA", "ILPF", nullptr));
        actionBLPF->setText(QApplication::translate("LIDA", "BLPF", nullptr));
        actionGLPF->setText(QApplication::translate("LIDA", "GLPF", nullptr));
        actionIHPF->setText(QApplication::translate("LIDA", "IHPF", nullptr));
        actionBHPF->setText(QApplication::translate("LIDA", "BHPF", nullptr));
        actionGHPF->setText(QApplication::translate("LIDA", "GHPF", nullptr));
        actionLaplacian_sharpening_fourier->setText(QApplication::translate("LIDA", "Laplacian_sharpening_fourier", nullptr));
        actionHigh_frequency_emphasis->setText(QApplication::translate("LIDA", "High_frequency_emphasis", nullptr));
        actionHomomorphic->setText(QApplication::translate("LIDA", "Homomorphic", nullptr));
        actionSelective_filtering->setText(QApplication::translate("LIDA", "Selective_filtering", nullptr));
        actionload_Hdr->setText(QApplication::translate("LIDA", "load Hdr", nullptr));
        actionload_Panorama->setText(QApplication::translate("LIDA", "load Panorama", nullptr));
        actionTest->setText(QApplication::translate("LIDA", "Test", nullptr));
        actionTest2->setText(QApplication::translate("LIDA", "Test2", nullptr));
        actionTest3->setText(QApplication::translate("LIDA", "Test3", nullptr));
        menuFile->setTitle(QApplication::translate("LIDA", "File", nullptr));
        menuAbout->setTitle(QApplication::translate("LIDA", "Affine", nullptr));
        menutest->setTitle(QApplication::translate("LIDA", "Intensity_transform", nullptr));
        menutest3->setTitle(QApplication::translate("LIDA", "Spatial_filtering", nullptr));
        menuFourier_transform->setTitle(QApplication::translate("LIDA", "Fourier_transform", nullptr));
        menuAbout_2->setTitle(QApplication::translate("LIDA", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LIDA: public Ui_LIDA {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIDA_H
