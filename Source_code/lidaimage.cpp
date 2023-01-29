#include "lidaimage.h"

LidaImage::LidaImage(int row, int col)
{
    this->row = row;
    this->col = col;

    matrix3D = new float**[3];
    for(int i = 0; i < 3; ++i){
        matrix3D[i] = new float *[row];
        for(int j = 0; j < row; ++j){
            matrix3D[i][j] = new float[col];
            for(int k = 0; k < col; ++k){
                matrix3D[i][j][k] = 0;
            }
        }
    }
}

LidaImage::~LidaImage()
{
    for(int i = 0; i < 3; ++i){
        for(int j=0; j < row; ++j){
            delete [] matrix3D[i][j];
        }
        delete [] matrix3D[i];
    }
    delete [] matrix3D;
    matrix3D = 0;
    //    cout << "delete" << endl;
}

LidaImage *LidaImage::grayscale()
{
    LidaImage* output = new LidaImage(this->row, this->col);
    for (int x = 0; x < this->row; x++) {
        for (int y = 0; y < this->col; y++) {
            float average = this->matrix3D[0][x][y] + this->matrix3D[1][x][y] + this->matrix3D[2][x][y];
            average = average/3;
            output->matrix3D[0][x][y] = average;
            output->matrix3D[1][x][y] = average;
            output->matrix3D[2][x][y] = average;
        }
    }
    return  output;
}

LidaImage* LidaImage::RGB2HSI()
{
     LidaImage* output = new LidaImage(this->row, this->col);
     float theta = 0;
     float S = 0;
     float I = 0;
//     for (int x = 0; x < row; x++) {
//         for (int y = 0; y < col; y++) {
//             for (int channel = 0; channel < 3; channel++) {
//                 this->matrix3D[channel][x][y] = this->matrix3D[channel][x][y]/255;
//             }
//         }
//     }
     for (int x = 0; x < row; x++) {
         for (int y = 0; y < col; y++) {
             float R = this->matrix3D[0][x][y]/255;
             float G = this->matrix3D[1][x][y]/255;
             float B = this->matrix3D[2][x][y]/255;
             theta = (std::acos(0.5*((R-G)+(R-B))/(std::sqrt((R-G)*(R-G)+(R-B)*(G-B))+0.0001)))*(180/3.1415926);
             if(B > G){
                 theta = 360 - theta;
             }
             float min = R;
             if(min > G){
                 min = G;
             }
             if (min > B) {
                 min = B;
             }
             S = 1 - (3*min)/(R+G+B);
             I = (R+G+B)/3;

             output->matrix3D[0][x][y] = theta;
             output->matrix3D[1][x][y] = S;
             output->matrix3D[2][x][y] = I;
         }
     }
     return output;
}

LidaImage *LidaImage::HSI2RGB()
{
    LidaImage* output = new LidaImage(this->row, this->col);
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < col; y++) {
            float H = this->matrix3D[0][x][y];
            float S = this->matrix3D[1][x][y];
            float I = this->matrix3D[2][x][y];
            float R = 0;
            float G = 0;
            float B = 0;
            if((H >= 0) && (H < 120)){
                B = I*(1-S);
                R = I*(1+(S*(std::cos(H*(3.1415926/180)))/(std::cos((60-H)*(3.1415926/180)))));
                G = 3*I-(R+B);
            }
            else if ((H >= 120) && (H < 240)) {
                H = H - 120;
                R = I*(1-S);
                G = I*(1+(S*(std::cos(H*(3.1415926/180)))/(std::cos((60-H)*(3.1415926/180)))));
                B = 3*I-(R+G);
            }
            else if ((H >= 240) && (H <= 360)) {
                H = H - 240;
                G = I*(1-S);
                B = I*(1+(S*(std::cos(H*(3.1415926/180)))/(std::cos((60-H)*(3.1415926/180)))));
                R = 3*I-(G+B);
            }
            R = R*255;
            G = G*255;
            B = B*255;
            if(R > 255){
                output->matrix3D[0][x][y] = 255;
            }
            else if (R < 0) {
                output->matrix3D[0][x][y] = 0;
            }
            else {
                output->matrix3D[0][x][y] = R;
            }

            if(G > 255){
                output->matrix3D[1][x][y] = 255;
            }
            else if (G < 0) {
                output->matrix3D[1][x][y] = 0;
            }
            else {
                output->matrix3D[1][x][y] = G;
            }

            if(B > 255){
                output->matrix3D[2][x][y] = 255;
            }
            else if (B < 0) {
                output->matrix3D[2][x][y] = 0;
            }
            else {
                output->matrix3D[2][x][y] = B;
            }
        }
    }
    return output;
}

void LidaImage::scaling(LidaImage::Calibration calibration)
{
    if(calibration == LidaImage::cutoff){
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                for (int channel = 0; channel < 3; channel++) {
                    if(matrix3D[channel][x][y] > 255){
                        matrix3D[channel][x][y] = 255;
                    }
                    else if (matrix3D[channel][x][y] < 0) {
                        matrix3D[channel][x][y] = 0;
                    }
                }
            }
        }
    }
    else if (calibration == LidaImage::full_range) {
        float R_min = matrix3D[0][0][0];
        float G_min = matrix3D[1][0][0];
        float B_min = matrix3D[2][0][0];
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                if(matrix3D[0][x][y] < R_min){
                    R_min = matrix3D[0][x][y];
                }
                if(matrix3D[1][x][y] < G_min){
                    G_min = matrix3D[1][x][y];
                }
                if(matrix3D[2][x][y] < B_min){
                    B_min = matrix3D[2][x][y];
                }
            }
        }
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                matrix3D[0][x][y] = matrix3D[0][x][y] - R_min;
                matrix3D[1][x][y] = matrix3D[1][x][y] - G_min;
                matrix3D[2][x][y] = matrix3D[2][x][y] - B_min;
            }
        }
        const int K = 255;
        float R_max = matrix3D[0][0][0];
        float G_max = matrix3D[1][0][0];
        float B_max = matrix3D[2][0][0];
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                if(matrix3D[0][x][y] > R_max){
                    R_max = matrix3D[0][x][y];
                }
                if(matrix3D[1][x][y] > G_max){
                    G_max = matrix3D[1][x][y];
                }
                if(matrix3D[2][x][y] > B_max){
                    B_max = matrix3D[2][x][y];
                }
            }
        }
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                matrix3D[0][x][y] = K*(matrix3D[0][x][y]/R_max);
                matrix3D[1][x][y] = K*(matrix3D[1][x][y]/G_max);
                matrix3D[2][x][y] = K*(matrix3D[2][x][y]/B_max);
            }
        }
    }
}

LidaImage *LidaImage::addGaussianNoise(float mean, float std)
{
    LidaImage* output = new LidaImage(this->row, this->col);

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, std);
    double gaussian_noise = 0;
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            gaussian_noise = distribution(generator);
            output->matrix3D[0][x][y] = this->matrix3D[0][x][y] + gaussian_noise;
            output->matrix3D[1][x][y] = this->matrix3D[1][x][y] + gaussian_noise;
            output->matrix3D[2][x][y] = this->matrix3D[2][x][y] + gaussian_noise;
        }
    }

    return output;
}

float LidaImage::PSNR(LidaImage *input, LidaImage *benchmark, int k)
{
    float MSE = 0;
    float PSNR = 0;
    float MAX_I = std::pow(2, k) - 1;
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            MSE += std::pow(input->matrix3D[0][x][y] - benchmark->matrix3D[0][x][y],2);
            MSE += std::pow(input->matrix3D[1][x][y] - benchmark->matrix3D[1][x][y],2);
            MSE += std::pow(input->matrix3D[2][x][y] - benchmark->matrix3D[2][x][y],2);
        }
    }
    MSE /= (3*(input->row)*(input->col));
    PSNR = 20 * std::log10(MAX_I*MAX_I/MSE);

    return PSNR;
}

float LidaImage::SSIM(LidaImage *input, LidaImage *benchmark, int k)
{
    //Note: This function applies on whole image,
    //      with sliding windows approach could be more accurate
    float SSIM = 0;
    float mean_x = 0;
    float mean_y = 0;
    float variance_x = 0;
    float variance_y = 0;
    float covariance = 0;
    float MN = (input->row) * (input->col);
    float L = std::pow(2, k) - 1;
    float c1 = (0.01*L)*(0.01*L);
    float c2 = (0.03*L)*(0.03*L);
//    cout << "L = " << L << endl;

    LidaImage* HSI_input = input->RGB2HSI();
    LidaImage* HSI_benchmark = benchmark->RGB2HSI();

    for (int x = 0; x < HSI_input->row; x++) {
        for (int y = 0; y < HSI_input->col; y++) {
            mean_x += HSI_input->matrix3D[2][x][y];
        }
    }
    mean_x /= MN;

    for (int x = 0; x < HSI_input->row; x++) {
        for (int y = 0; y < HSI_input->col; y++) {
            variance_x += std::pow(HSI_input->matrix3D[2][x][y] - mean_x, 2);
        }
    }
    variance_x /= (MN - 1);

//    cout << "mean_x = " << mean_x << endl;
//    cout << "variance_x = " << variance_x << endl;



    for (int x = 0; x < HSI_benchmark->row; x++) {
        for (int y = 0; y < HSI_benchmark->col; y++) {
            mean_y += HSI_benchmark->matrix3D[2][x][y];
        }
    }
    mean_y /= MN;

    for (int x = 0; x < HSI_benchmark->row; x++) {
        for (int y = 0; y < HSI_benchmark->col; y++) {
            variance_y += std::pow(HSI_benchmark->matrix3D[2][x][y] - mean_y, 2);
        }
    }
    variance_y /= (MN - 1);

//    cout << "mean_y = " << mean_y << endl;
//    cout << "variance_y = " << variance_y << endl;


    for (int x = 0; x < HSI_input->row; x++) {
        for (int y = 0; y < HSI_input->col; y++) {
            covariance += (HSI_input->matrix3D[2][x][y] - mean_x)*(HSI_benchmark->matrix3D[2][x][y] - mean_y);
        }
    }
    covariance /= (MN - 1);

//    cout << "covariance = " << covariance << endl;

    SSIM = ((2*mean_x*mean_y + c1)*(2*covariance + c2)) / ((mean_x*mean_x + mean_y*mean_y +c1)*(variance_x + variance_y + c2));

    delete HSI_input;
    delete HSI_benchmark;
    return SSIM;
}
