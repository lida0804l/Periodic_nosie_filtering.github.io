#include "hdr.h"

Hdr::Hdr()
{

}

LidaImage *Hdr::create()
{
    LidaImage* output;

    QStringList Filepaths;
    QFileDialog dialog;
    dialog.setWindowTitle("Choose the HDR components");
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
        image_sets[i] = Hdr::QImage2LidaImage(image);
        delete image;
    }

    string Filepath = (QFileDialog::getOpenFileName(NULL, "Choose the exposure time file",NULL,"Excel (*.txt)")).toStdString();//--Input csv File
    ifstream inFile;
    string line;
    inFile.open(Filepath.c_str());
    std::vector<float> exposure;
    int lines_to_read = image_sets.size();
    for (int i = 0; i < lines_to_read; i++) {
        getline(inFile,line);
        exposure.push_back(std::log(std::stof(line)));
        // cout << std::log(2.718) << endl;
    }
//    for (int i = 0; i < exposure.size(); i++) {
//        cout << exposure[i] << endl;
//    }

    float w[256];
    int Z_min = 0;
    int Z_max = 255;
    for (int i = 0; i < 256; i++) {
        if(i <= 0.5*(Z_min + Z_max)){
            w[i] = i - Z_min;
        }
        else {
            w[i] = Z_max - i;
        }
//        cout << w[i] << endl;
    }

    srand (time(NULL));
    int P = image_sets.size();
    int N = float(255)/(P-1); // Number of samples for pixel values
    N = N*3; // N(P-1) > Zmax - Zmin = 255
//    cout << N << endl;
    output = new LidaImage(image_sets[0]->row, image_sets[0]->col);


// For recording sampled coordinates //////////////////////////////////////////////////////////////////////////////////////////////////////
//    int **sample_coordinates = new int*[N];
//    for(int i = 0; i < N; i++){
//        sample_coordinates[i] = new int[2];
//        int x = rand() % (image_sets[0]->row);
//        int y = rand() % (image_sets[0]->col);
//        sample_coordinates[i][0] = x;
//        sample_coordinates[i][1] = y;

//        cout << x << " " << y << endl;
//    }
//    cout << endl;

//    QString savepath = QFileDialog::getSaveFileName(NULL, "Save sampled coordinates", NULL, "Text_file (*.txt)");
//    ofstream outFile;
//    outFile.open((savepath.toStdString()).c_str());

//    for (int i = 0; i < N; i++) {
//        outFile << sample_coordinates[i][0] << "," << sample_coordinates[i][1] << endl;
//    }
//    outFile.close();

//    for(int i = 0; i < N; i++)
//        delete[] sample_coordinates[i];
//    delete[] sample_coordinates;
// //////////////////////////////////////////////////////////////////////////////////////////////////////

    // For loading sampled coordinates //////////////////////////////////////////////////////////////////////////////////////////////////////

    int **sample_coordinates = new int*[N];
    for(int i = 0; i < N; i++){
        sample_coordinates[i] = new int[2];
        // cout << x << " " << y << endl;
    }
    // cout << endl;

    string Filepath_alt = (QFileDialog::getOpenFileName(NULL, "Choose sampled coordinates",NULL,"Text_file (*.txt)")).toStdString();//--Input csv File
    ifstream inFile_alt;
    string line_alt;
    string line_part;
    inFile_alt.open(Filepath_alt.c_str());
    float histogramSpecified_I[256];
    int i = 0;
    while(getline(inFile_alt,line))
    {
        istringstream in(line);
        getline(in, line_part, ',');
        sample_coordinates[i][0] = std::stof(line_part);
        // cout << line_part << endl;

        getline(in, line_part, ',');
        sample_coordinates[i][1] = std::stof(line_part);
        // cout << line_part << endl;

        i++;
    }
    inFile.close();

    // //////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int channel = 0; channel < 3; channel++) {
        std::vector<std::vector<int>> Z;
        for (int i = 0; i < N; i++) {
            // Tmp for sampled coordinates
            int x = sample_coordinates[i][0];
            int y = sample_coordinates[i][1];
            // //////////////////////////////////////////////////////////////////////////////////////////////////////
//            int x = rand() % (image_sets[0]->row);
//            int y = rand() % (image_sets[0]->col);

            //        cout << x << " " << y << endl;
            std::vector<int> tmp;
            for (int j = 0; j < P; j++) {
                tmp.push_back(int(image_sets[j]->matrix3D[channel][x][y]));
            }
            Z.push_back(tmp);
        }
        std::vector<float> x = Hdr::g_solve(Z, exposure, 100, w);

        // Output solved g(x) and ln(E)

        QString savepath = QFileDialog::getSaveFileName(NULL, "Save solved g(x) and ln(E)", NULL, "EXCEL (*.csv)");
        ofstream outFile;
        outFile.open((savepath.toStdString()).c_str());

        cout << x.size() << endl;
        for (int i = 0; i < x.size(); i++) {
            outFile << x[i] << endl;
        }
        outFile.close();

        // //////////////////////////////////////////////////////////////////////////////////////////////////////



        //    cout << x.size() << endl;
//            for (int i = 0; i < 256; i++) {
//                cout << x[i] << endl;
//            }
//        cout << output->row << " " << output->col << endl;
        for (int i = 0; i < output->row; i++) {
            for (int j = 0; j < output->col; j++) {
                float numerator = 0;
                float denominator = 0;
                for (int p = 0; p < P; p++) {
                    numerator += (w[int(image_sets[p]->matrix3D[channel][i][j])]) * (x[int(image_sets[p]->matrix3D[channel][i][j])] - exposure[p]);
                    denominator += w[int(image_sets[p]->matrix3D[channel][i][j])];
                }
                if(denominator < 0.0001){
                    output->matrix3D[channel][i][j] = std::exp(0);
                }
                else {
                    output->matrix3D[channel][i][j] = std::exp(numerator / denominator);
                }
//                cout << output->matrix3D[channel][i][j] << " " << numerator << " " << denominator << "    ";
            }
        }
//        cout << endl;
    }
    float avg_Lw_r = 0;
    float avg_Lw_g = 0;
    float avg_Lw_b = 0;
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            avg_Lw_r += std::log(0.0001 + output->matrix3D[0][x][y]);
            avg_Lw_g += std::log(0.0001 + output->matrix3D[1][x][y]);
            avg_Lw_b += std::log(0.0001 + output->matrix3D[2][x][y]);
//            for (int channel = 0; channel < 3; channel++) {

//            }
        }
    }
    avg_Lw_r = avg_Lw_r/(output->row * output->col);
    avg_Lw_g = avg_Lw_g/(output->row * output->col);
    avg_Lw_b = avg_Lw_b/(output->row * output->col);
    avg_Lw_r = std::exp(avg_Lw_r);
    avg_Lw_g = std::exp(avg_Lw_g);
    avg_Lw_b = std::exp(avg_Lw_b);

//    cout << avg_Lw_r << " " << avg_Lw_g << " " << avg_Lw_b << endl;

    float alpha = 0.18;
    float Lm_white = 0.5;
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            float Lm_r = (alpha / avg_Lw_r) * output->matrix3D[0][x][y];
            float Ld_r = Lm_r / (1 + Lm_r);
//            float Ld_r = Lm_r * (1 + Lm_r / (Lm_white*Lm_white)) / (1 + Lm_r);

            float Lm_g = (alpha / avg_Lw_g) * output->matrix3D[1][x][y];
            float Ld_g = Lm_g / (1 + Lm_g);
//            float Ld_g = Lm_g * (1 + Lm_g / (Lm_white*Lm_white)) / (1 + Lm_g);

            float Lm_b = (alpha / avg_Lw_b) * output->matrix3D[2][x][y];
            float Ld_b = Lm_b / (1 + Lm_b);
//            float Ld_b = Lm_b * (1 + Lm_b / (Lm_white*Lm_white)) / (1 + Lm_b);

            output->matrix3D[0][x][y] = Ld_r;
            output->matrix3D[1][x][y] = Ld_g;
            output->matrix3D[2][x][y] = Ld_b;
        }
    }
    for (int i = 0; i < image_sets.size(); i++) {
        delete image_sets[i];
    }

    return output;
}

std::vector<float> Hdr::g_solve(std::vector<std::vector<int> > Z, std::vector<float> B, float l, float w[])
{
    std::vector<float> x;

    int n = 256;
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < Z.size() * Z.at(0).size() + n + 1; i++) {
        vector< double > temp(n + Z.size(), 0);
        A.push_back(temp);
    }
    b.resize(A.size());

//    for (int i = 0; i < 256; i++) {
//        cout << w[i] << endl;
//    }

    int k = 0;
    for (int i = 0; i < Z.size(); i++) {
        for (int j = 0; j < Z.at(0).size()  ; j++) {
            double wij = w[Z[i][j]];
            A[k][Z[i][j]] = wij;
            A[k][n + i] = -wij;

            b[k] = wij * B[j] ;
            k++;
//            cout << A[k][Z[i][j]] << " " << A[k][n + i] << " " << b[k] << " ";
//            cout << Z[i][j] << " ";
//            cout << Z[i][j] << " " << w[Z[i][j]] << " " << endl;
        }
//        cout << endl;
    }

    A[k][128] = 1;
    k++;

//    for (int i = 0; i < n-1; i++) {
//        A[k][i] = l * w[i+1];
//        A[k][i + 1] = -2 * l * w[i+1];
//        A[k][i + 2] = l * w[i+1];
//        k++;
//    }

    for (int i = 0; i < n-2; i++) { // From 0 ~ 253
        A[k][i] = l * w[i+1];
        A[k][i + 1] = -2 * l * w[i+1];
        A[k][i + 2] = l * w[i+1];
        k++;
    }


//    cout << A.size() << " " << A[0].size() << endl;
//    for (int i = 0; i < A.size(); i++) {
//        for (int j = 0; j < A[0].size(); j++) {
////            cout << "run here" << endl;
////            A[i][j];
//            cout << A[i][j] << " ";
//        }
//        cout << endl;
//    }

    LidaMatrix AA(A.size(), A[0].size());
    for (int i = 0; i < AA.row; i++) {
        for (int j = 0; j < AA.col; j++) {
            AA(i,j) = A[i][j];
        }
    }
    LidaMatrix bb(b.size(),1);
    for (int i = 0; i < bb.row; i++) {
        bb(i,0) = b[i];
//        cout << bb(i,0) << endl;
    }
    LidaMatrix transpose = AA.transpose();
    LidaMatrix inverse = (transpose * AA).inverse();
    LidaMatrix xx = inverse * transpose * bb;
//    cout << inverse.row << " " << inverse.col << endl;
//    cout << xx.row << " " << xx.col << endl;
//    for (int i = 0; i < n; i++) {
//        cout << std::exp(xx(i,0)) << endl;
//    }
//    for (int i = 0; i < n; i++) {
    for (int i = 0; i < xx.row; i++) {
        x.push_back(xx(i,0));
    }

    return x;
}

LidaImage *Hdr::QImage2LidaImage(QImage *input)
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

QImage *Hdr::LidaImage2QImage(LidaImage *input)
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
