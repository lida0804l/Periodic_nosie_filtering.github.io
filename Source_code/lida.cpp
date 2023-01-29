#include "lida.h"
#include "ui_lida.h"

LIDA::LIDA(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LIDA)
{
    ui->setupUi(this);

    input = 0;
//    image = new QImage();

    scene = new LidaScene();
    pixmap_item = new QGraphicsPixmapItem();
}

LIDA::~LIDA()
{
    delete ui;
}

void LIDA::on_actionload_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "Load Image", 0, "Image Files(*.jpg *.bmp *.png *tif)");
    if(!(fileName.isNull())){
        if(input != 0){
            delete input;
            cout << "image contained" << endl;
        }
        QImage* image = new QImage();
        image->load(fileName);
        // scene->lidaimage = input;
        // scene->image = image;

        *image = image->convertToFormat(QImage::Format_RGB888);
        input = LIDA::QImage2LidaImage(image);
        scene->lidaimage = input;
        LIDA::showImage(image);
        delete image;
    }
}

void LIDA::showImage(QImage* imageDisplay)
{
    pixmap_item->setPixmap(QPixmap::fromImage(*imageDisplay));

    QList<QGraphicsItem *> itemList = scene->items();
    if(itemList.contains(pixmap_item))
    {
        scene->removeItem(pixmap_item);
    }
    scene->pixmap_item = pixmap_item;
    scene->addItem(pixmap_item);
    scene->setSceneRect(pixmap_item->boundingRect());
    ui->graphicsView->setScene(scene);
}

LidaImage* LIDA::QImage2LidaImage(QImage *input)
{
    LidaImage* output = new LidaImage(input->height(), input->width());
    for (int x = 0; x < input->height(); x++) {
        for (int y = 0; y < input->width(); y++) {
            int r = input->pixelColor(y,x).red();
            int g = input->pixelColor(y,x).green();
            int b = input->pixelColor(y,x).blue();
            output->matrix3D[0][x][y] = r;
            output->matrix3D[1][x][y] = g;
            output->matrix3D[2][x][y] = b;
        }
    }
    return output;
}

QImage* LIDA::LidaImage2QImage(LidaImage *input)
{
    QImage* output = new QImage(input->col, input->row, QImage::Format_RGB888);
    for (int x = 0; x < output->height(); x++) {
        for (int y = 0; y < output->width(); y++) {
            int r = input->matrix3D[0][x][y];
            int g = input->matrix3D[1][x][y];
            int b = input->matrix3D[2][x][y];
            output->setPixel(y,x,qRgb(r,g,b));
        }
    }
    return output;
}

void LIDA::on_actionScaling_triggered()
{
    bool get_Scaling = false;
    double Scaling = QInputDialog::getDouble(this, tr("Set scaling factor"),
                                 tr("Scaling factor:"), 1, 0, 10, 1, &get_Scaling);
    LidaMatrix* mask = new LidaMatrix(3,3);
    mask->operator()(0,0) = Scaling;
    mask->operator()(1,1) = Scaling;
    mask->operator()(2,2) = 1;

    QStringList items;
    items << tr("Nearest neighbor") << tr("Bilinear") << tr("Bicubic");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set interpolation"),
                                             tr("interpolation:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Nearest neighbor"){
        output = Affine::transformation(input, mask, Scaling, Affine::NearestNeighbor);
    }
    else if (item.toStdString() == "Bilinear") {
        output = Affine::transformation(input, mask, Scaling, Affine::Bilinear);
    }
    else if (item.toStdString() == "Bicubic") {
        output = Affine::transformation(input, mask, Scaling, Affine::Bicubic);
    }

    delete input;
    input = output;
    delete mask;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionRotation_triggered()
{
    bool get_Rotation = false;
    int Rotation = QInputDialog::getDouble(this, tr("Set rotation angle"),
                                 tr("rotation angle:"), 60, -360, 360, 1, &get_Rotation);
    float c = cos(-3.14/(180.0/Rotation));
    float s = sin(-3.14/(180.0/Rotation));
    LidaMatrix translate(3,3);
    translate(0,0) = 1;
    translate(1,1) = 1;
    translate(2,2) = 1;
    translate(2,0) = -1 * input->row/2;
    translate(2,1) = -1 * input->col/2;
    LidaMatrix rotate_mask(3,3);
    rotate_mask(0,0) = c;
    rotate_mask(0,1) = s;
    rotate_mask(1,0) = -1*s;
    rotate_mask(1,1) = c;
    rotate_mask(2,2) = 1;
    LidaMatrix translate_back(3,3);
    translate_back(0,0) = 1;
    translate_back(1,1) = 1;
    translate_back(2,2) = 1;
    translate_back(2,0) = input->row/2;
    translate_back(2,1) = input->col/2;
    LidaMatrix tmp = translate * rotate_mask * translate_back;

    LidaMatrix* mask = new LidaMatrix(3,3);
    for(int i = 0; i < mask->row; i++){
        for(int j = 0; j < mask->col; j++){
            mask->operator()(i,j) = tmp(i,j);
        }
    }

    QStringList items;
    items << tr("Nearest neighbor") << tr("Bilinear") << tr("Bicubic");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set interpolation"),
                                             tr("interpolation:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Nearest neighbor"){
        output = Affine::transformation(input, mask, 1, Affine::NearestNeighbor);
    }
    else if (item.toStdString() == "Bilinear") {
        output = Affine::transformation(input, mask, 1, Affine::Bilinear);
    }
    else if (item.toStdString() == "Bicubic") {
        output = Affine::transformation(input, mask, 1, Affine::Bicubic);
    }

    delete input;
    input = output;
    delete mask;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionTranslation_triggered()
{
    bool get_Translation = false;
    double Translation = QInputDialog::getDouble(this, tr("Set Translation factor"),
                                 tr("Translation factor:"), 10, -1000, 1000, 1, &get_Translation);
    LidaMatrix* mask = new LidaMatrix(3,3);
    mask->operator()(0,0) = 1;
    mask->operator()(1,1) = 1;
    mask->operator()(2,2) = 1;
    mask->operator()(2,0) = Translation;
    mask->operator()(2,1) = Translation;

    QStringList items;
    items << tr("Nearest neighbor") << tr("Bilinear") << tr("Bicubic");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set interpolation"),
                                             tr("interpolation:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Nearest neighbor"){
        output = Affine::transformation(input, mask, 1, Affine::NearestNeighbor);
    }
    else if (item.toStdString() == "Bilinear") {
        output = Affine::transformation(input, mask, 1, Affine::Bilinear);
    }
    else if (item.toStdString() == "Bicubic") {
        output = Affine::transformation(input, mask, 1, Affine::Bicubic);
    }

    delete input;
    input = output;
    delete mask;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionShear_vertical_triggered()
{
    bool get_Shear_vertical = false;
    double Shear_vertical = QInputDialog::getDouble(this, tr("Set Shear factor (vertical)"),
                                 tr("Shear factor(vertical):"), 5, -5, 10, 1, &get_Shear_vertical);
    LidaMatrix* mask = new LidaMatrix(3,3);
    mask->operator()(0,0) = 1;
    mask->operator()(1,1) = 1;
    mask->operator()(2,2) = 1;
    mask->operator()(1,0) = Shear_vertical;

    QStringList items;
    items << tr("Nearest neighbor") << tr("Bilinear") << tr("Bicubic");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set interpolation"),
                                             tr("interpolation:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Nearest neighbor"){
        output = Affine::transformation(input, mask, 1, Affine::NearestNeighbor);
    }
    else if (item.toStdString() == "Bilinear") {
        output = Affine::transformation(input, mask, 1, Affine::Bilinear);
    }
    else if (item.toStdString() == "Bicubic") {
        output = Affine::transformation(input, mask, 1, Affine::Bicubic);
    }

    delete input;
    input = output;
    delete mask;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionShear_horizontal_triggered()
{
    bool get_Shear_horizontal = false;
    double Shear_horizontal = QInputDialog::getDouble(this, tr("Set Shear factor (horizontal)"),
                                 tr("Shear factor(horizontal):"), 5, -5, 10, 1, &get_Shear_horizontal);
    LidaMatrix* mask = new LidaMatrix(3,3);
    mask->operator()(0,0) = 1;
    mask->operator()(1,1) = 1;
    mask->operator()(2,2) = 1;
    mask->operator()(0,1) = Shear_horizontal;

    QStringList items;
    items << tr("Nearest neighbor") << tr("Bilinear") << tr("Bicubic");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set interpolation"),
                                             tr("interpolation:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Nearest neighbor"){
        output = Affine::transformation(input, mask, 1, Affine::NearestNeighbor);
    }
    else if (item.toStdString() == "Bilinear") {
        output = Affine::transformation(input, mask, 1, Affine::Bilinear);
    }
    else if (item.toStdString() == "Bicubic") {
        output = Affine::transformation(input, mask, 1, Affine::Bicubic);
    }

    delete input;
    input = output;
    delete mask;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionsave_as_triggered()
{
    QImage* result = LIDA::LidaImage2QImage(input);
    QString savepath = QFileDialog::getSaveFileName(this, tr("Save File"), NULL, tr("jpg (*.jpg);; bmp (*.bmp);; png (*.png)"));
    result->save(savepath);
    delete result;
}

void LIDA::on_actionHistogram_equalization_triggered()
{
    LidaImage* output = IntensityTransformation::histogram_equalization(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionHistogram_specification_triggered()
{
    LidaImage* output = IntensityTransformation::histogram_specification(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionFuzzy_enhancement_triggered()
{
    LidaImage* output = IntensityTransformation::fuzzy(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionGaussian_triggered()
{
    LidaImage* output = SpatialFiltering::gaussian(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionLaplacian_triggered()
{
    LidaImage* output = SpatialFiltering::laplacian(input);
    output->scaling(LidaImage::full_range);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionLaplacian_sharpening_triggered()
{
    LidaImage* output = SpatialFiltering::laplacianSharpening(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionHighboost_sharpening_triggered()
{
    bool press_yes = false;
    int k = QInputDialog::getDouble(this, tr("Set sharpening factor k"),
                                 tr("Sharpening factor k:"), 1, 0, 10, 1, &press_yes);
    LidaImage* output = SpatialFiltering::highboostSharpening(input, k);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionSobel_triggered()
{
    LidaImage* output = SpatialFiltering::sobel(input);
    output->scaling(LidaImage::full_range);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionFuzzy_edge_triggered()
{
    LidaImage* output = SpatialFiltering::fuzzy(input);
    output->scaling(LidaImage::full_range);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionILPF_triggered()
{
    bool press_yes = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 100, 1, &press_yes);
    LidaImage* output = FourierTransform::ILPF(input, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionBLPF_triggered()
{
    bool press_yes = false;
    int n = QInputDialog::getDouble(this, tr("Set order n"),
                                 tr("Order n:"), 2, 1, 10, 1, &press_yes);
    bool press_yes2 = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 100, 1, &press_yes2);
    LidaImage* output = FourierTransform::BLPF(input, 2, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionGLPF_triggered()
{
    bool press_yes = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 10000, 1, &press_yes);
    LidaImage* output = FourierTransform::GLPF(input, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionIHPF_triggered()
{
    bool press_yes = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 100, 1, &press_yes);
    LidaImage* output = FourierTransform::IHPF(input, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionBHPF_triggered()
{
    bool press_yes = false;
    int n = QInputDialog::getDouble(this, tr("Set order n"),
                                 tr("Order n:"), 2, 1, 10, 1, &press_yes);
    bool press_yes2 = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 100, 1, &press_yes2);
    LidaImage* output = FourierTransform::BHPF(input, 2, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionGHPF_triggered()
{
    bool press_yes = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 60, 0, 100, 1, &press_yes);
    LidaImage* output = FourierTransform::GHPF(input, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionLaplacian_sharpening_fourier_triggered()
{
    LidaImage* output = FourierTransform::laplacianSharpening(input);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionHigh_frequency_emphasis_triggered()
{
    bool press_yes = false;
    double k1 = QInputDialog::getDouble(this, tr("Set k1"),
                                 tr("k1:"), 1, 0, 10, 1, &press_yes);
    bool press_yes2 = false;
    double k2 = QInputDialog::getDouble(this, tr("Set k2"),
                                 tr("k2:"), 0.75, 0, 10, 1, &press_yes2);
    bool press_yes3 = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 40, 0, 10000, 1, &press_yes3);
    LidaImage* output = FourierTransform::high_frequency_emphasis(input, k1, k2, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionHomomorphic_triggered()
{
    bool press_yes = false;
    double gamma_H = QInputDialog::getDouble(this, tr("Set gamma_H"),
                                 tr("gamma_H:"), 2, 0, 10, 1, &press_yes);
    bool press_yes2 = false;
    double gamma_L = QInputDialog::getDouble(this, tr("Set gamma_L"),
                                 tr("gamma_L:"), 0.75, 0, 10, 1, &press_yes2);
    bool press_yes3 = false;
    int c = QInputDialog::getDouble(this, tr("Set c"),
                                 tr("c:"), 1, 0, 10, 1, &press_yes3);
    bool press_yes4 = false;
    int D0 = QInputDialog::getDouble(this, tr("Set cutoff frequency D0"),
                                 tr("Cutoff frequency D0:"), 80, 0, 100, 1, &press_yes4);
    LidaImage* output = FourierTransform::homomorphic(input, gamma_H, gamma_L, c, D0);
    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionSelective_filtering_triggered()
{
    QStringList items;
    items << tr("Ideal rect") << tr("Butterworth circle");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set selection type"),
                                             tr("selection type:"), items, 0, false, &ok);

    LidaImage* output;
    if(item.toStdString() == "Ideal rect"){
        output = FourierTransform::selective_filtering(input, FourierTransform::ideal_rect);
    }
    else if (item.toStdString() == "Butterworth circle") {
        output = FourierTransform::selective_filtering(input, FourierTransform::butterworth_circle);
    }

    output->scaling(LidaImage::cutoff);
    delete input;
    input = output;
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionload_Hdr_triggered()
{
    if(input != 0){
        delete input;
        cout << "image contained" << endl;
    }
    input = Hdr::create();
    input->scaling(LidaImage::full_range);
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionload_Panorama_triggered()
{
    if(input != 0){
        delete input;
        cout << "image contained" << endl;
    }
    input = Panorama::stitch();
    QImage* result = LIDA::LidaImage2QImage(input);
    LIDA::showImage(result);
    delete result;
}

void LIDA::on_actionTest_triggered()
{

    // 1. Harris corner
//    std::vector<std::vector<Panorama::Descriptor>> feature_sets(1);
//    LidaImage* corner_img = Panorama::harris_corner(input, feature_sets[0]);

//    delete input;
//    input = corner_img;
//    input->scaling(LidaImage::cutoff);
////    input->scaling(LidaImage::full_range);
//    QImage* result = LIDA::LidaImage2QImage(input);
//    scene->lidaimage = input;
//    LIDA::showImage(result);

// ////////////////////////////////////////////////////////


    // 2. Cylindrical warping
//    LidaImage* Output = Panorama::warping(input, 770);

//    delete input;
//    input = Output;
//    input->scaling(LidaImage::cutoff);
//    QImage* result = LIDA::LidaImage2QImage(input);
//    scene->lidaimage = input;
//    LIDA::showImage(result);
// ////////////////////////////////////////////////////////

    //



    QString savepath = QFileDialog::getSaveFileName(NULL, "Save sift descriptor", NULL, "Excel (*.csv)");
    ofstream outFile;
    outFile.open((savepath.toStdString()).c_str());

    LidaImage* Output = input->grayscale();

    for (int x = 0; x < input->row; x++) {
            for (int y = 0; y < input->col; y++) {
                outFile << Output->matrix3D[0][x][y] << ",";
        }
        outFile << endl;
    }


    outFile << endl;
    outFile.close();





// ////////////////////////////////////////////////////////




//    LidaImage* output = input->addGaussianNoise(0, 50);
//    output->scaling(LidaImage::cutoff);

//    float PSNR = LidaImage::PSNR(input, output, 8);
//    cout << "PSNR: " << PSNR << endl;
//    float SSIM = LidaImage::SSIM(input, output);
//    cout << "SSIM = " << SSIM << endl;

//    delete input;
//    input = output;
//    QImage* result = LIDA::LidaImage2QImage(input);
//    scene->lidaimage = input;
//    LIDA::showImage(result);

// ////////////////////////////////////////////////////////

//    LidaImage* benchmark;
//    LidaImage* input;

//    QString fileName = QFileDialog::getOpenFileName(NULL, "Load benchmark image", 0, "Image Files(*.jpg *.bmp *.png *tif)");
//    if(!(fileName.isNull())){
//        QImage* image = new QImage();
//        image->load(fileName);
//        *image = image->convertToFormat(QImage::Format_RGB888);
//        benchmark = LIDA::QImage2LidaImage(image);
//        LIDA::showImage(image);
//        delete image;
//    }
//    QString fileName_2 = QFileDialog::getOpenFileName(NULL, "Load input image", 0, "Image Files(*.jpg *.bmp *.png *tif)");
//    if(!(fileName.isNull())){
//        QImage* image = new QImage();
//        image->load(fileName_2);
//        *image = image->convertToFormat(QImage::Format_RGB888);
//        input = LIDA::QImage2LidaImage(image);
//        LIDA::showImage(image);
//        delete image;
//    }
//    float PSNR = LidaImage::PSNR(input, benchmark, 8);
//    cout << "PSNR: " << PSNR << endl;

// ////////////////////////////////////////////////////////////////////

//    LidaImage* output = SpatialFiltering::average(input, 99);
//    delete input;
//    input = output;
//    QImage* result = LIDA::LidaImage2QImage(input);
//    scene->lidaimage = input;
//    LIDA::showImage(result);
}

void LIDA::on_actionTest2_triggered()
{

    //    LidaImage* output = new LidaImage();
        QStringList Filepaths;
        QFileDialog dialog;
        dialog.setWindowTitle("Choose the panorama components");
        dialog.setNameFilter("Image Files(*.jpg *.bmp *.png *tif)");
        dialog.setFileMode(QFileDialog::ExistingFiles);
        if(dialog.exec()){
            Filepaths = dialog.selectedFiles();
        }
        std::vector<LidaImage*> image_sets(Filepaths.size());
        for(int i = 0; i < Filepaths.length(); i++){
            QImage *image = new QImage();
            image->load(Filepaths.at(i));
            *image = image->convertToFormat(QImage::Format_RGB888);
            image_sets[i] = QImage2LidaImage(image);
            delete image;
        }
        std::vector<std::vector<Panorama::Descriptor>> feature_sets(image_sets.size());
        for(int i = 0; i < Filepaths.size(); i++){
            LidaImage* corner_img = Panorama::harris_corner(image_sets[i], feature_sets[i]);
            delete corner_img;
        }

    //    for (int i = 0; i < feature_sets.size(); i++) {
    //        cout << feature_sets[i].size() << endl;
    //    }

    //    Match_pair match_pair = Panorama::comparison();
    //    Panorama::Match_pair match_pair = Panorama::comparison(feature_sets[0], feature_sets[1]);
    //    cout << match_pair.left.size() << " " << match_pair.right.size() << endl;
    //    Panorama::outlier_removal(image_sets[0], image_sets[1], match_pair);

        string Filepath = (QFileDialog::getOpenFileName(NULL, "Choose the focal length file",NULL,"Excel (*.txt)")).toStdString();//--Input csv File
        ifstream inFile;
        string line;
        inFile.open(Filepath.c_str());
        int lines_to_read = 13*image_sets.size();
        std::vector<float> focal_lengths;
        for (int i = 0; i < lines_to_read; i++) {
            getline(inFile,line);
            if(((i-11)%13) == 0){
                focal_lengths.push_back(std::stof(line));
            }
        }
    //    for (int i = 0; i < focal_lengths.size(); i++) {
    //        cout << focal_lengths[i] << endl;
    //    }

        // //////////////////////////////////////////////////////////
        //      image warping done yet (cylindrical transform)     //
        for (int i = 0; i < image_sets.size(); i++) {
            LidaImage* tmp = image_sets[i];
            image_sets[i] = Panorama::warping(tmp, focal_lengths[i]);
            delete tmp;
        }
        // //////////////////////////////////////////////////////////

        LidaImage* pano;

        Panorama::Match_pair match_pair;

        int x0 = 0;
        int y0 = 0;
        for (int i = 0; i < image_sets.size(); i++) {
            if(i == 0){
                pano = new LidaImage(image_sets[0]->row, image_sets[0]->col);
                for (int x = 0; x < pano->row; x++) {
                    for (int y = 0; y < pano->col; y++) {
                        for (int channel = 0; channel < 3; channel++) {
                            pano->matrix3D[channel][x][y] = image_sets[0]->matrix3D[channel][x][y];
                        }
                    }
                }
            }
            else {
                match_pair = Panorama::comparison(feature_sets[i-1], feature_sets[i]);
                Panorama::outlier_removal(image_sets[i-1], image_sets[i], match_pair);

                cout << "Amount of feature matches: " << match_pair.left.size() << endl;

                int fx0 = match_pair.left[match_pair.left.size()/2].x;
                int fy0 = match_pair.left[match_pair.left.size()/2].y;
                int fx1 = match_pair.right[match_pair.right.size()/2].x;
                int fy1 = match_pair.right[match_pair.right.size()/2].y;

                int mid_x = image_sets[i-1]->col/2;
                int mid_y = image_sets[i-1]->row/2;
                double theta = atan((fy0-mid_x)/focal_lengths[i-1]);
                double h = (fx0-mid_y)/pow(pow((fy0-mid_x),2)+pow(focal_lengths[i-1],2),0.5);
                fy0 = focal_lengths[i-1]*theta+mid_x;
                fx0 = focal_lengths[i-1]*h+mid_y;

                mid_x = image_sets[i]->col/2;
                mid_y = image_sets[i]->row/2;
                theta = atan((fy1-mid_x)/focal_lengths[i]);
                h = (fx1-mid_y)/pow(pow((fy1-mid_x),2)+pow(focal_lengths[i],2),0.5);
                fy1 = focal_lengths[i]*theta+mid_x;
                fx1 = focal_lengths[i]*h+mid_y;

                int r0 = image_sets[i-1]->row;
                int c0 = image_sets[i-1]->col;
                int r1 = image_sets[i]->row;
                int c1 = image_sets[i]->col;
                LidaImage* tmp;
                if((x0 + fx0 - fx1) < 0){
                    cout << "case 2" << endl;
    //                cout << (x0 + fx0 - fx1) << endl;
    //                x0 = x0 + std::abs(x0 + fx0 - fx1);
                    tmp = new LidaImage(pano->row + std::abs(x0 + fx0 - fx1), c1 + (y0 + fy0 - fy1));
                    cout << pano->row << "," << pano->col << endl;
                    cout << pano->row + std::abs(x0 + fx0 - fx1) << "," << c1 + y0 + fy0 - fy1 << endl;
    //                cout << "run here" << endl;
    //                x0 = x0 + std::abs(x0 + fx0 - fx1);
    //                cout << "run here" << endl;
    //                cout << pano->row << " " << pano->col << endl;
    //                cout << tmp->row << " " << tmp->col << endl;
                    for (int x = 0; x < pano->row; x++) {
    //                    cout << x + x0 << " ";
                        for (int y = 0; y < pano->col; y++) {
    //                        cout << "(" << x+std::abs(x0 + fx0 - fx1) << "," << y << ") ";
                            for (int channel = 0; channel < 3; channel++) {
                                tmp->matrix3D[channel][x+std::abs(x0 + fx0 - fx1)][y] = pano->matrix3D[channel][x][y];
                            }
                        }
    //                    cout << endl;
    //                    cout << x + x0 << " ";
                    }
                    x0 = x0 + std::abs(x0 + fx0 - fx1);
    //                x0 = x0 + fx0 - fx1;
    //                cout << "end loop2" << endl;
                }
                else {
                    cout << "case 1" << endl;
    //                if(i == 3){
    //                    cout << "i == 3 run here" << endl;
    //                }
                    if((r1 + x0 + fx0 - fx1) <= pano->row){
                        tmp = new LidaImage(pano->row, c1 + y0 + fy0 - fy1);
    //                    cout << "1" << endl;
                    }
                    else {
                        tmp = new LidaImage(r1 + x0 + fx0 - fx1, c1 + y0 + fy0 - fy1);
    //                    cout << "2" << endl;
                    }
    //                tmp = new LidaImage(r1 + x0 + fx0 - fx1, c1 + y0 + fy0 - fy1);
    //                cout << pano->row << "," << pano->col << endl;
    //                cout << tmp->row << "," << tmp->col << endl;
    //                cout << r1 + x0 + fx0 - fx1 << "," << c1 + y0 + fy0 - fy1 << endl;
                    for (int x = 0; x < pano->row; x++) {
                        for (int y = 0; y < pano->col; y++) {
                            for (int channel = 0; channel < 3; channel++) {
                                tmp->matrix3D[channel][x][y] = pano->matrix3D[channel][x][y];
                            }
                        }
                    }
    //                if(i == 1){
    //                    cout << "i == 1 run here" << endl;
    //                }
                }
    //            if(i == 3){
    //                cout << "i == 3 run here" << endl;
    //            }
                for (int x = x0 + fx0 - fx1; x < (x0 + fx0 - fx1 + r1); x++) {
                    for (int y = y0 + fy0 - fy1; y < (y0 + fy0 - fy1 + c1); y++) {
    //                    cout << x - (x0 + fx0 - fx1) << " " << y - (y0 + fy0 - fy1) << endl;
    //                    cout << x << " " << y << endl;
                        if(((x < (x0 + r0)) && (x >= (x0 + fx0 - fx1))) && ((y < (y0 + c0)) && (y >= (y0 + fy0 - fy1)))){
    //                        cout << x << " " << y << " " << x0 + fx0 - fx1 << " " <<

                            if((tmp->matrix3D[0][x][y] == 0) && (tmp->matrix3D[1][x][y] == 0) && (tmp->matrix3D[2][x][y] == 0)){
                                for (int channel = 0; channel < 3; channel++) {
                                    tmp->matrix3D[channel][x][y] = image_sets[i]->matrix3D[channel][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)];
                                }
                            }
                            else if ((image_sets[i]->matrix3D[0][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)] == 0) && (image_sets[i]->matrix3D[1][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)] == 0) && (image_sets[i]->matrix3D[2][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)] == 0)) {
                                for (int channel = 0; channel < 3; channel++) {
                                    tmp->matrix3D[channel][x][y] = tmp->matrix3D[channel][x][y];
                                }
                            }
                            else {
                                float weight = 1 - (1.0 / (c0 - (fy0 - fy1))) * (y - (y0 + fy0 + - fy1));
                                for (int channel = 0; channel < 3; channel++) {
                                    tmp->matrix3D[channel][x][y] = weight * (tmp->matrix3D[channel][x][y]) + (1 - weight) * (image_sets[i]->matrix3D[channel][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)]);
                                }
                            }

    //                        float weight = 1 - (1.0 / (c0 - (fy0 - fy1))) * (y - (y0 + fy0 + - fy1));
    //                        //                            cout << weight << " ";
    //                        for (int channel = 0; channel < 3; channel++) {
    //                            tmp->matrix3D[channel][x][y] = weight * (tmp->matrix3D[channel][x][y]) + (1 - weight) * (image_sets[i]->matrix3D[channel][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)]);
    //                        }
                        }
                        else {
                            for (int channel = 0; channel < 3; channel++) {
                                tmp->matrix3D[channel][x][y] = image_sets[i]->matrix3D[channel][x - (x0 + fx0 - fx1)][y - (y0 + fy0 - fy1)];
                            }
                        }
                    }
                }
                delete pano;
                pano = tmp;
                x0 = x0 + fx0 - fx1;
                y0 = y0 + fy0 - fy1;
    //            cout << "x0: " << x0 << " yo: " << y0 << endl;
            }
        }

        // pano = Panorama::draw_feature_match(image_sets[0], image_sets[1], match_pair);

        QString savepath = QFileDialog::getSaveFileName(NULL, "Save feature matches", NULL, "Excel (*.csv)");
        ofstream outFile;
        outFile.open((savepath.toStdString()).c_str());

        for (int i = 0; i < match_pair.left.size(); i++) {
            outFile << match_pair.left[i].x << "," << match_pair.left[i].y << ",";
            for(int j = 0; j < 128; j++){
                outFile << match_pair.left[i].hist[j] << ",";
            }
            outFile << endl;
        }
        outFile.close();




        QImage* result = Panorama::draw_feature_match(image_sets[0], image_sets[1], match_pair);


        for (int i = 0; i < image_sets.size(); i++) {
            delete image_sets[i];
        }

        scene->lidaimage = input;
        LIDA::showImage(result);

//        // return pano;

}

void LIDA::on_actionTest3_triggered()
{
//    LidaImage* output = new LidaImage(16, 16);

//    int x = 292;
//    int y = 547;

//    int idx_i = 0;
//    int idx_j = 0;

//    for (int i = -8; i < 8; i++) {
//        for (int j = -8; j < 8; j++) {
//            output->matrix3D[0][idx_i][idx_j] = input->matrix3D[0][x+i][y+j];
//            output->matrix3D[1][idx_i][idx_j] = input->matrix3D[1][x+i][y+j];
//            output->matrix3D[2][idx_i][idx_j] = input->matrix3D[2][x+i][y+j];
//            idx_j++;
//        }
//        idx_j = 0;
//        idx_i++;
//    }

//    LidaImage* output = new LidaImage(input->row, input->col);

    LidaImage* magnitude = SpatialFiltering::gradient_magnitude(input);
    LidaImage* angle = SpatialFiltering::gradient_angle(input);
    float weight[16][16];
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            float D2 = (-7.5+x)*(-7.5+x) + (-7.5+y)*(-7.5+y);
            weight[x][y] = std::exp(-1*D2/(2*2.4*2.4));
        }
    }


    QVector<float> histogram(10);
    QVector<float> histogram_sorted(10);
    for (int i = 0; i < histogram.size(); i++) {
        histogram[i] = 0;
        histogram_sorted[i] = 0;
    }
    int idx_i = 0;
    int idx_j = 0;

    int x = 292;
    int y = 547;

    for (int i = -8; i < 8; i++) {
        for (int j = -8; j < 8; j++) {
            int bins = int(angle->matrix3D[0][x+i][y+j]);
            bins += 22.5;
            if(bins < 0){
                bins += 360;
            }
            bins = bins%360;
            bins = bins/45;

            histogram[bins] += weight[idx_i][idx_j]*magnitude->matrix3D[0][x][y];
            histogram_sorted[bins] = histogram[bins];
            idx_j++;
        }
        idx_j = 0;
        idx_i++;
    }
    qSort(histogram_sorted.begin(), histogram_sorted.end(), qGreater<float>());

//    for (int i = 0; i < 10; i++) {
//        cout << histogram.at(i) << endl;
//    }

    float theta = (histogram.indexOf(histogram_sorted.at(0)))*45;
//    float theta = 0;

    cout << "theta: " << theta << endl;

    Panorama::Descriptor a;
    a.x = x;
    a.y = y;
    idx_i = 0;
    idx_j = 0;

    for (int i = -8; i < 8; i++) {
        for (int j = -8; j < 8; j++) {
            int x_rotate = std::round(i*std::cos(theta) - j*std::sin(theta));
            int y_rotate = std::round(i*std::sin(theta) + j*std::cos(theta));
            int bins = int(angle->matrix3D[0][x+x_rotate][y+y_rotate] - theta);

            bins += 22.5;
            if(bins < 0){
                bins += 360;
            }
            bins = bins%360;
            bins = bins/45;
            a.hist[(idx_i/4)*32 + (idx_j/4)*8 + bins] += weight[idx_i][idx_j]*magnitude->matrix3D[0][x+x_rotate][y+y_rotate];
            idx_j++;

//            cout << bins*45 << "," << magnitude->matrix3D[0][x+x_rotate][y+y_rotate] << endl;
        }
        idx_j = 0;
        idx_i++;
//        cout << endl;
    }

//    for (int i = 0; i < 128; i++) {
//        cout << a.hist[i] << endl;
//    }

//    QString savepath = QFileDialog::getSaveFileName(NULL, "Save sift descriptor", NULL, "Excel (*.csv)");
//    ofstream outFile;
//    outFile.open((savepath.toStdString()).c_str());

//    for (int i = 0; i < 128; i++) {
//        outFile << a.hist[i] << ",";
//    }
//    outFile << endl;
//    outFile.close();

    LidaImage* output = magnitude;
    output->scaling(LidaImage::full_range);
    delete input;
    input = output;

    LidaImage* crop = new LidaImage(16, 16);

//    int x = 292;
//    int y = 547;

    idx_i = 0;
    idx_j = 0;

    for (int i = -8; i < 8; i++) {
        for (int j = -8; j < 8; j++) {
            crop->matrix3D[0][idx_i][idx_j] = input->matrix3D[0][x+i][y+j];
            crop->matrix3D[1][idx_i][idx_j] = input->matrix3D[1][x+i][y+j];
            crop->matrix3D[2][idx_i][idx_j] = input->matrix3D[2][x+i][y+j];
            idx_j++;
        }
        idx_j = 0;
        idx_i++;
    }

    delete input;
    input = crop;

    QImage* result = LIDA::LidaImage2QImage(input);
    scene->lidaimage = input;
    LIDA::showImage(result);

}
