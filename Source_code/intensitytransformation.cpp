#include "intensitytransformation.h"

IntensityTransformation::IntensityTransformation()
{

}

LidaImage* IntensityTransformation::histogram_equalization(LidaImage *input)
{
    LidaImage* HSI = input->RGB2HSI();
    int histogram_I[256];
    for (int i = 0; i < 256; i++) {
        histogram_I[i] = 0;
    }
    for (int x = 0; x < HSI->row; x++) {
        for (int y = 0; y < HSI->col; y++) {
            histogram_I[int((HSI->matrix3D[2][x][y])*255)]++;
        }
    }

    const int L = 256;
    int s_I[256];
    for (int k = 0; k < 256; k++) {
        float tmp_I = 0;
        for (int i = 0; i <= k; i++) {
            tmp_I += histogram_I[i];
        }
        tmp_I = (L-1)*tmp_I/((HSI->row)*(HSI->col));
        if((tmp_I-int(tmp_I)) > 0.5){
            tmp_I = int(tmp_I)+1;
        }
        else {
            tmp_I = int(tmp_I);
        }
        s_I[k] = tmp_I;
    }
    for (int x = 0; x < HSI->row; x++) {
        for (int y = 0; y < HSI->col; y++) {
            int b = s_I[int((HSI->matrix3D[2][x][y])*255)];
            HSI->matrix3D[2][x][y] = b/255.0;
        }
    }
    LidaImage* output = HSI->HSI2RGB();
    delete HSI;
    return output;

    //Individual RGB plane processing
//    LidaImage* output = new LidaImage(input->row, input->col);
//    int histogram_Red[256];
//    int histogram_Green[256];
//    int histogram_Blue[256];
//    for (int i = 0; i < 256; i++) {
//        histogram_Red[i] = 0;
//        histogram_Green[i] = 0;
//        histogram_Blue[i] = 0;
//    }
//    for (int x = 0; x < input->row; x++) {
//        for (int y = 0; y < input->col; y++) {
//            histogram_Red[int(input->matrix3D[0][x][y])]++;
//            histogram_Green[int(input->matrix3D[1][x][y])]++;
//            histogram_Blue[int(input->matrix3D[2][x][y])]++;
//        }
//    }

//    const int L = 256;
//    int s_Red[256];
//    int s_Green[256];
//    int s_Blue[256];
//    for (int k = 0; k < 256; k++) {
//        float tmp_Red = 0;
//        float tmp_Green = 0;
//        float tmp_Blue = 0;
//        for (int i = 0; i <= k; i++) {
//            tmp_Red += histogram_Red[i];
//            tmp_Green += histogram_Green[i];
//            tmp_Blue += histogram_Blue[i];
//        }
//        tmp_Red = (L-1)*tmp_Red/((input->row)*(input->col));
//        tmp_Green = (L-1)*tmp_Green/((input->row)*(input->col));
//        tmp_Blue = (L-1)*tmp_Blue/((input->row)*(input->col));
//        if((tmp_Red-int(tmp_Red)) > 0.5){
//            tmp_Red = int(tmp_Red)+1;
//        }
//        else {
//            tmp_Red = int(tmp_Red);
//        }
//        if((tmp_Green-int(tmp_Green)) > 0.5){
//            tmp_Green = int(tmp_Green)+1;
//        }
//        else {
//            tmp_Green = int(tmp_Green);
//        }
//        if((tmp_Blue-int(tmp_Blue)) > 0.5){
//            tmp_Blue = int(tmp_Blue)+1;
//        }
//        else {
//            tmp_Blue = int(tmp_Blue);
//        }
//        s_Red[k] = tmp_Red;
//        s_Green[k] = tmp_Green;
//        s_Blue[k] = tmp_Blue;
//    }
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            int r = s_Red[int(input->matrix3D[0][x][y])];
//            int g = s_Green[int(input->matrix3D[1][x][y])];
//            int b = s_Blue[int(input->matrix3D[2][x][y])];
//            output->matrix3D[0][x][y] = r;
//            output->matrix3D[1][x][y] = g;
//            output->matrix3D[2][x][y] = b;
//        }
//    }

    //Histogram for RGB component images
//    int histogramEqual_Red[256];
//    int histogramEqual_Green[256];
//    int histogramEqual_Blue[256];
//    for (int i = 0; i < 256; i++) {
//        histogramEqual_Red[i] = 0;
//        histogramEqual_Green[i] = 0;
//        histogramEqual_Blue[i] = 0;
//    }
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            histogramEqual_Red[int(output->matrix3D[0][x][y])]++;
//            histogramEqual_Green[int(output->matrix3D[1][x][y])]++;
//            histogramEqual_Blue[int(output->matrix3D[2][x][y])]++;
//        }
//    }
//    QString savepath = QFileDialog::getSaveFileName(NULL, "Save histogram", NULL, "Excel (*.csv)");
//    ofstream outFile;
//    outFile.open((savepath.toStdString()).c_str());
//    for (int i = 0; i < 256; i++) {
//        outFile << histogram_Red[i] << "," << histogram_Green[i] << "," << histogram_Blue[i] << "," << " "
//             << histogramEqual_Red[i] << "," << histogramEqual_Green[i] << "," << histogramEqual_Blue[i] << "," << endl;
//    }
//    outFile.close();
//    return output;
}

LidaImage* IntensityTransformation::histogram_specification(LidaImage *input)
{
    LidaImage* HSI = input->RGB2HSI();
    int histogram_I[256];
    for (int i = 0; i < 256; i++) {
        histogram_I[i] = 0;
    }
    for (int x = 0; x < HSI->row; x++) {
        for (int y = 0; y < HSI->col; y++) {
            histogram_I[int((HSI->matrix3D[2][x][y])*255)]++;
        }
    }

    const int L = 256;
    int s_I[256];
    for (int k = 0; k < 256; k++) {
        float tmp_I = 0;
        for (int i = 0; i <= k; i++) {
            tmp_I += histogram_I[i];
        }
        tmp_I = (L-1)*tmp_I/((HSI->row)*(HSI->col));
        if((tmp_I-int(tmp_I)) > 0.5){
            tmp_I = int(tmp_I)+1;
        }
        else {
            tmp_I = int(tmp_I);
        }
        s_I[k] = tmp_I;
    }

    string Filepath = (QFileDialog::getOpenFileName(NULL, "Choose specified histogram",NULL,"Excel (*.csv)")).toStdString();//--Input csv File
    ifstream inFile;
    string line;
    string line_part;
    inFile.open(Filepath.c_str());
    float histogramSpecified_I[256];
    int i = 0;
    while(getline(inFile,line))
    {
        istringstream in(line);
        getline(in, line_part, ',');
        histogramSpecified_I[i] = std::stof(line_part);
        i++;
    }
    inFile.close();
    int sG_I[256];
    for (int k = 0; k < 256; k++) {
        float tmp_I = 0;
        for (int i = 0; i <= k; i++) {
            tmp_I += histogramSpecified_I[i];
        }
        tmp_I = (L-1)*tmp_I;
        if((tmp_I-int(tmp_I)) > 0.5){
            tmp_I = int(tmp_I)+1;
        }
        else {
            tmp_I = int(tmp_I);
        }
        sG_I[k] = tmp_I;
    }
    int sG_Inverse_I[256];
    for (int k = 0; k < 256; k++) {
        int difference = 256;
        for (int i = 0; i < 256; i++) {
            if(abs(k-sG_I[i]) < difference){
                sG_Inverse_I[k] = i;
                difference = abs(k-sG_I[i]);
            }
        }
    }
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            int b = sG_Inverse_I[s_I[int((HSI->matrix3D[2][x][y])*255)]];
            HSI->matrix3D[2][x][y] = b/255.0;
        }
    }
    LidaImage* output = HSI->HSI2RGB();
    delete HSI;
    return output;

//    LidaImage* output = new LidaImage(input->row, input->col);
//    int histogram_Red[256];
//    int histogram_Green[256];
//    int histogram_Blue[256];
//    for (int i = 0; i < 256; i++) {
//        histogram_Red[i] = 0;
//        histogram_Green[i] = 0;
//        histogram_Blue[i] = 0;
//    }
//    for (int x = 0; x < input->row; x++) {
//        for (int y = 0; y < input->col; y++) {
//            histogram_Red[int(input->matrix3D[0][x][y])]++;
//            histogram_Green[int(input->matrix3D[1][x][y])]++;
//            histogram_Blue[int(input->matrix3D[2][x][y])]++;
//        }
//    }

//    const int L = 256;
//    int s_Red[256];
//    int s_Green[256];
//    int s_Blue[256];
//    for (int k = 0; k < 256; k++) {
//        float tmp_Red = 0;
//        float tmp_Green = 0;
//        float tmp_Blue = 0;
//        for (int i = 0; i <= k; i++) {
//            tmp_Red += histogram_Red[i];
//            tmp_Green += histogram_Green[i];
//            tmp_Blue += histogram_Blue[i];
//        }
//        tmp_Red = (L-1)*tmp_Red/((input->row)*(input->col));
//        tmp_Green = (L-1)*tmp_Green/((input->row)*(input->col));
//        tmp_Blue = (L-1)*tmp_Blue/((input->row)*(input->col));
//        if((tmp_Red-int(tmp_Red)) > 0.5){
//            tmp_Red = int(tmp_Red)+1;
//        }
//        else {
//            tmp_Red = int(tmp_Red);
//        }
//        if((tmp_Green-int(tmp_Green)) > 0.5){
//            tmp_Green = int(tmp_Green)+1;
//        }
//        else {
//            tmp_Green = int(tmp_Green);
//        }
//        if((tmp_Blue-int(tmp_Blue)) > 0.5){
//            tmp_Blue = int(tmp_Blue)+1;
//        }
//        else {
//            tmp_Blue = int(tmp_Blue);
//        }
//        s_Red[k] = tmp_Red;
//        s_Green[k] = tmp_Green;
//        s_Blue[k] = tmp_Blue;
//    }

//    string Filepath = (QFileDialog::getOpenFileName(NULL, "Choose specified histogram",NULL,"Excel (*.csv)")).toStdString();//--Input csv File
//    ifstream inFile;
//    string line;
//    string line_part;
//    inFile.open(Filepath.c_str());
//    float histogramSpecified_Red[256];
//    float histogramSpecified_Green[256];
//    float histogramSpecified_Blue[256];
//    int i = 0;
//    while(getline(inFile,line))
//    {
//        istringstream in(line);
//        getline(in, line_part, ',');
//        histogramSpecified_Red[i] = std::stof(line_part);
//        getline(in, line_part, ',');
//        histogramSpecified_Green[i] = std::stof(line_part);
//        getline(in, line_part, ',');
//        histogramSpecified_Blue[i] = std::stof(line_part);
//        i++;
//    }
//    inFile.close();
//    int sG_Red[256];
//    int sG_Green[256];
//    int sG_Blue[256];
//    for (int k = 0; k < 256; k++) {
//        float tmp_Red = 0;
//        float tmp_Green = 0;
//        float tmp_Blue = 0;
//        for (int i = 0; i <= k; i++) {
//            tmp_Red += histogramSpecified_Red[i];
//            tmp_Green += histogramSpecified_Green[i];
//            tmp_Blue += histogramSpecified_Blue[i];
//        }
//        tmp_Red = (L-1)*tmp_Red;
//        tmp_Green = (L-1)*tmp_Green;
//        tmp_Blue = (L-1)*tmp_Blue;
//        if((tmp_Red-int(tmp_Red)) > 0.5){
//            tmp_Red = int(tmp_Red)+1;
//        }
//        else {
//            tmp_Red = int(tmp_Red);
//        }
//        if((tmp_Green-int(tmp_Green)) > 0.5){
//            tmp_Green = int(tmp_Green)+1;
//        }
//        else {
//            tmp_Green = int(tmp_Green);
//        }
//        if((tmp_Blue-int(tmp_Blue)) > 0.5){
//            tmp_Blue = int(tmp_Blue)+1;
//        }
//        else {
//            tmp_Blue = int(tmp_Blue);
//        }
//        sG_Red[k] = tmp_Red;
//        sG_Green[k] = tmp_Green;
//        sG_Blue[k] = tmp_Blue;
//    }
//    int sG_Inverse_Red[256];
//    int sG_Inverse_Green[256];
//    int sG_Inverse_Blue[256];
//    for (int k = 0; k < 256; k++) {
//        int difference = 256;
//        for (int i = 0; i < 256; i++) {
//            if(abs(k-sG_Red[i]) < difference){
//                sG_Inverse_Red[k] = i;
//                difference = abs(k-sG_Red[i]);
//            }
//        }
//    }
//    for (int k = 0; k < 256; k++) {
//        int difference = 256;
//        for (int i = 0; i < 256; i++) {
//            if(abs(k-sG_Green[i]) < difference){
//                sG_Inverse_Green[k] = i;
//                difference = abs(k-sG_Green[i]);
//            }
//        }
//    }
//    for (int k = 0; k < 256; k++) {
//        int difference = 256;
//        for (int i = 0; i < 256; i++) {
//            if(abs(k-sG_Red[i]) < difference){
//                sG_Inverse_Blue[k] = i;
//                difference = abs(k-sG_Red[i]);
//            }
//        }
//    }
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            int r = s_Red[int(input->matrix3D[0][x][y])];
//            int g = s_Green[int(input->matrix3D[1][x][y])];
//            int b = s_Blue[int(input->matrix3D[2][x][y])];
//            output->matrix3D[0][x][y] = r;
//            output->matrix3D[1][x][y] = g;
//            output->matrix3D[2][x][y] = b;
//        }
//    }
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            int r = sG_Inverse_Blue[s_Red[int(input->matrix3D[0][x][y])]];
//            int g = sG_Inverse_Green[s_Green[int(input->matrix3D[1][x][y])]];
//            int b = sG_Inverse_Blue[s_Blue[int(input->matrix3D[2][x][y])]];
//            output->matrix3D[0][x][y] = r;
//            output->matrix3D[1][x][y] = g;
//            output->matrix3D[2][x][y] = b;
//        }
//    }
    //    return output;
}

//LidaImage *IntensityTransformation::grayscale(LidaImage *input)
//{
//    LidaImage* output = new LidaImage(input->row, input->col);
//    for (int x = 0; x < input->row; x++) {
//        for (int y = 0; y < input->col; y++) {
//            float average = input->matrix3D[0][x][y] + input->matrix3D[1][x][y] + input->matrix3D[2][x][y];
//            average = average/3;
//            output->matrix3D[0][x][y] = average;
//            output->matrix3D[1][x][y] = average;
//            output->matrix3D[2][x][y] = average;
//        }
//    }
//    return  output;
//}

LidaImage *IntensityTransformation::fuzzy(LidaImage *input)
{
    LidaImage* HSI = input->RGB2HSI();
    float u_dark[256];
    float u_gray[256];
    float u_bright[256];
    for (int i = 0; i < 256; i++) {
        if((i >= 0) && (i <= 63)){
            u_dark[i] = 1;
        }
        if((i >= 64) && (i <= 127)){
            u_dark[i] = (1.0/64)*(-1*i + 127);
        }
        if((i >= 128) && (i <= 255)){
            u_dark[i] = 0;
        }
    }
    for (int i = 0; i < 256; i++) {
        if((i >= 0) && (i <= 63)){
            u_gray[i] = 0;
        }
        if((i >= 64) && (i <= 127)){
            u_gray[i] = (1.0/64)*(i - 63);
        }
        if((i >= 128) && (i <= 191)){
            u_gray[i] = (1.0/64)*(-1*i + 191);
        }
        if((i >= 192) && (i <= 255)){
            u_gray[i] = 0;
        }
    }
    for (int i = 0; i < 256; i++) {
        if((i >= 0) && (i <= 127)){
            u_bright[i] = 0;
        }
        if((i >= 128) && (i <= 191)){
            u_bright[i] = (1.0/64)*(i - 127);
        }
        if((i >= 192) && (i <= 255)){
            u_bright[i] = 1;
        }
    }
//    for (int i = 0; i < 256; i++) {
//        cout << u_dark[i] << " " << u_gray[i] << " " << u_bright[i] << " " << endl;
//    }

    int vd = 0;
    int vg = 127;
    int vb = 255;
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            float I = (HSI->matrix3D[2][x][y])*255;
            HSI->matrix3D[2][x][y] = (u_dark[int(I)]*vd + u_gray[int(I)]*vg + u_bright[int(I)]*vb) / (u_dark[int(I)] + u_gray[int(I)] + u_bright[int(I)]);
            HSI->matrix3D[2][x][y] = (HSI->matrix3D[2][x][y]) / 255;
        }
    }
    LidaImage* output = HSI->HSI2RGB();
    delete HSI;
    return output;

//    float u_dark[256];
//    float u_gray[256];
//    float u_bright[256];
//    for (int i = 0; i < 256; i++) {
//        if((i >= 0) && (i <= 63)){
//            u_dark[i] = 1;
//        }
//        if((i >= 64) && (i <= 127)){
//            u_dark[i] = (1.0/64)*(-1*i + 127);
//        }
//        if((i >= 128) && (i <= 255)){
//            u_dark[i] = 0;
//        }
//    }
//    for (int i = 0; i < 256; i++) {
//        if((i >= 0) && (i <= 63)){
//            u_gray[i] = 0;
//        }
//        if((i >= 64) && (i <= 127)){
//            u_gray[i] = (1.0/64)*(i - 63);
//        }
//        if((i >= 128) && (i <= 191)){
//            u_gray[i] = (1.0/64)*(-1*i + 191);
//        }
//        if((i >= 192) && (i <= 255)){
//            u_gray[i] = 0;
//        }
//    }
//    for (int i = 0; i < 256; i++) {
//        if((i >= 0) && (i <= 127)){
//            u_bright[i] = 0;
//        }
//        if((i >= 128) && (i <= 191)){
//            u_bright[i] = (1.0/64)*(i - 127);
//        }
//        if((i >= 192) && (i <= 255)){
//            u_bright[i] = 1;
//        }
//    }
////    for (int i = 0; i < 256; i++) {
////        cout << u_dark[i] << " " << u_gray[i] << " " << u_bright[i] << " " << endl;
////    }

//    int vd = 0;
//    int vg = 127;
//    int vb = 255;
//    LidaImage* output = new LidaImage(input->row, input->col);
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            output->matrix3D[0][x][y] = (u_dark[int(input->matrix3D[0][x][y])]*vd + u_gray[int(input->matrix3D[0][x][y])]*vg + u_bright[int(input->matrix3D[0][x][y])]*vb) / (u_dark[int(input->matrix3D[0][x][y])] + u_gray[int(input->matrix3D[0][x][y])] + u_bright[int(input->matrix3D[0][x][y])]);
//            output->matrix3D[1][x][y] = (u_dark[int(input->matrix3D[1][x][y])]*vd + u_gray[int(input->matrix3D[1][x][y])]*vg + u_bright[int(input->matrix3D[1][x][y])]*vb) / (u_dark[int(input->matrix3D[1][x][y])] + u_gray[int(input->matrix3D[1][x][y])] + u_bright[int(input->matrix3D[1][x][y])]);
//            output->matrix3D[2][x][y] = (u_dark[int(input->matrix3D[2][x][y])]*vd + u_gray[int(input->matrix3D[2][x][y])]*vg + u_bright[int(input->matrix3D[2][x][y])]*vb) / (u_dark[int(input->matrix3D[2][x][y])] + u_gray[int(input->matrix3D[2][x][y])] + u_bright[int(input->matrix3D[2][x][y])]);
//        }
//    }
//    return output;
}
