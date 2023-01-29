#include "spatialfiltering.h"

SpatialFiltering::SpatialFiltering()
{

}

LidaImage* SpatialFiltering::convolution(LidaImage *input, LidaMatrix *mask)
{
    LidaImage* output = new LidaImage(input->row, input->col);
    LidaImage* zero_padding = new LidaImage(input->row + 2*(mask->col-1), input->col + 2*(mask->row-1));
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            zero_padding->matrix3D[0][x][y] = 0;
            zero_padding->matrix3D[1][x][y] = 0;
            zero_padding->matrix3D[2][x][y] = 0;
        }
    }
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            int r = input->matrix3D[0][x][y];
            int g = input->matrix3D[1][x][y];
            int b = input->matrix3D[2][x][y];

            zero_padding->matrix3D[0][x+(mask->row-1)][y+(mask->col-1)] = r;
            zero_padding->matrix3D[1][x+(mask->row-1)][y+(mask->col-1)] = g;
            zero_padding->matrix3D[2][x+(mask->row-1)][y+(mask->col-1)] = b;
        }
    }
    LidaImage* zero_padding_alter = new LidaImage(zero_padding->row, zero_padding->col);
    for (int x = 0; x < zero_padding_alter->row; x++) {
        for (int y = 0; y < zero_padding_alter->col; y++) {
            zero_padding_alter->matrix3D[0][x][y] = zero_padding->matrix3D[0][x][y];
            zero_padding_alter->matrix3D[1][x][y] = zero_padding->matrix3D[1][x][y];
            zero_padding_alter->matrix3D[2][x][y] = zero_padding->matrix3D[2][x][y];
        }
    }
    for (int x = (mask->row-1)/2; x < (zero_padding->row-(mask->row-1)/2); x++) {
        for (int y = (mask->col-1)/2; y < (zero_padding->col-(mask->col-1)/2); y++) {
            float sum_of_product_Red = 0;
            float sum_of_product_Green = 0;
            float sum_of_product_Blue = 0;
            for (int i = 0; i < mask->row; i++) {
                for (int j = 0; j < mask->col; j++) {
                    int r = zero_padding->matrix3D[0][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j];
                    int g = zero_padding->matrix3D[1][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j];
                    int b = zero_padding->matrix3D[2][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j];
                    sum_of_product_Red += r*mask->operator()(i,j);
                    sum_of_product_Green += g*mask->operator()(i,j);
                    sum_of_product_Blue += b*mask->operator()(i,j);
                }
            }
            zero_padding_alter->matrix3D[0][x][y] = sum_of_product_Red;
            zero_padding_alter->matrix3D[1][x][y] = sum_of_product_Green;
            zero_padding_alter->matrix3D[2][x][y] = sum_of_product_Blue;
        }
    }
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            int r = zero_padding_alter->matrix3D[0][x+(mask->row-1)][y+(mask->col-1)];
            int g = zero_padding_alter->matrix3D[1][x+(mask->row-1)][y+(mask->col-1)];
            int b = zero_padding_alter->matrix3D[2][x+(mask->row-1)][y+(mask->col-1)];
            output->matrix3D[0][x][y] = r;
            output->matrix3D[1][x][y] = g;
            output->matrix3D[2][x][y] = b;
        }
    }
    delete zero_padding;
    delete zero_padding_alter;
    return output;
}

LidaImage *SpatialFiltering::non_maximum_supression(LidaImage* input)
{
    LidaImage* output = new LidaImage(input->row, input->col);
    LidaMatrix* mask = new LidaMatrix(3,3);
    LidaImage* zero_padding = new LidaImage(input->row + 2*(mask->col-1), input->col + 2*(mask->row-1));
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            zero_padding->matrix3D[0][x][y] = 0;
            zero_padding->matrix3D[1][x][y] = 0;
            zero_padding->matrix3D[2][x][y] = 0;
        }
    }
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            int r = input->matrix3D[0][x][y];
            int g = input->matrix3D[1][x][y];
            int b = input->matrix3D[2][x][y];

            zero_padding->matrix3D[0][x+(mask->row-1)][y+(mask->col-1)] = r;
            zero_padding->matrix3D[1][x+(mask->row-1)][y+(mask->col-1)] = g;
            zero_padding->matrix3D[2][x+(mask->row-1)][y+(mask->col-1)] = b;
        }
    }
    LidaImage* zero_padding_alter = new LidaImage(zero_padding->row, zero_padding->col);
    for (int x = 0; x < zero_padding_alter->row; x++) {
        for (int y = 0; y < zero_padding_alter->col; y++) {
            zero_padding_alter->matrix3D[0][x][y] = zero_padding->matrix3D[0][x][y];
            zero_padding_alter->matrix3D[1][x][y] = zero_padding->matrix3D[1][x][y];
            zero_padding_alter->matrix3D[2][x][y] = zero_padding->matrix3D[2][x][y];
        }
    }
    for (int x = (mask->row-1)/2; x < (zero_padding->row-(mask->row-1)/2); x++) {
        for (int y = (mask->col-1)/2; y < (zero_padding->col-(mask->col-1)/2); y++) {
//            bool not_max = true;
            for (int i = 0; i < mask->row; i++) {
                for (int j = 0; j < mask->col; j++) {
                    if((zero_padding->matrix3D[0][x][y]) < zero_padding->matrix3D[0][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j]){
                        zero_padding_alter->matrix3D[0][x][y] = 0;
                    }
//                    else {
//                        zero_padding_alter->matrix3D[0][x][y] = zero_padding->matrix3D[0][x][y];
//                    }
                    if((zero_padding->matrix3D[1][x][y]) < zero_padding->matrix3D[1][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j]){
                        zero_padding_alter->matrix3D[1][x][y] = 0;
                    }
//                    else {
//                        zero_padding_alter->matrix3D[1][x][y] = zero_padding->matrix3D[1][x][y];
//                    }
                    if((zero_padding->matrix3D[2][x][y]) < zero_padding->matrix3D[2][x-(mask->row-1)/2+i][y-(mask->col-1)/2+j]){
                        zero_padding_alter->matrix3D[2][x][y] = 0;
                    }
//                    else {
//                        zero_padding_alter->matrix3D[2][x][y] = zero_padding->matrix3D[2][x][y];
//                    }
                }
            }
        }
    }
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            int r = zero_padding_alter->matrix3D[0][x+(mask->row-1)][y+(mask->col-1)];
            int g = zero_padding_alter->matrix3D[1][x+(mask->row-1)][y+(mask->col-1)];
            int b = zero_padding_alter->matrix3D[2][x+(mask->row-1)][y+(mask->col-1)];
            output->matrix3D[0][x][y] = r;
            output->matrix3D[1][x][y] = g;
            output->matrix3D[2][x][y] = b;
        }
    }
    delete mask;
    delete zero_padding;
    delete zero_padding_alter;
    return output;
}

LidaImage *SpatialFiltering::average(LidaImage *input, int size)
{
    LidaMatrix* mask = new LidaMatrix(size, size);
    int MN = size * size;
    for(int i = 0; i < mask->row; i++){
        for(int j = 0; j < mask->col; j++){
            mask->operator()(i,j) = 1.0/MN;
        }
    }
    LidaImage* output = convolution(input, mask);
    delete mask;
    return output;
}

LidaImage *SpatialFiltering::gaussian(LidaImage *input)
{
    float matrix[3][3] = {{1.0/16, 2.0/16, 1.0/16},
                          {2.0/16, 4.0/16, 2.0/16},
                          {1.0/16, 2.0/16, 1.0/16}};
    LidaMatrix* mask = new LidaMatrix(3,3);
    for(int i = 0; i < mask->row; i++){
        for(int j = 0; j < mask->col; j++){
            mask->operator()(i,j) = matrix[i][j];
        }
    }
    LidaImage* output = convolution(input, mask);
    delete mask;
    return output;
}

LidaImage* SpatialFiltering::laplacian(LidaImage *input)
{
    float matrix[3][3] = {{1, 1, 1},
                          {1, -8, 1},
                          {1, 1, 1}};
    LidaMatrix* mask = new LidaMatrix(3,3);
    for(int i = 0; i < mask->row; i++){
        for(int j = 0; j < mask->col; j++){
            mask->operator()(i,j) = matrix[i][j];
        }
    }
    LidaImage* output = convolution(input, mask);
    delete mask;
    return output;
}

LidaImage* SpatialFiltering::laplacianSharpening(LidaImage *input)
{
    LidaImage* second_derivative = SpatialFiltering::laplacian(input);
    const int c = -1;
    LidaImage* output = new LidaImage(input->row, input->col);
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            output->matrix3D[0][x][y] = (input->matrix3D[0][x][y]) + c*(second_derivative->matrix3D[0][x][y]);
            output->matrix3D[1][x][y] = (input->matrix3D[1][x][y]) + c*(second_derivative->matrix3D[1][x][y]);
            output->matrix3D[2][x][y] = (input->matrix3D[2][x][y]) + c*(second_derivative->matrix3D[2][x][y]);
        }
    }
    delete second_derivative;
    return output;
}

LidaImage* SpatialFiltering::highboostSharpening(LidaImage *input, int k)
{
    float matrix[5][5];
    float std = 3;
    for(int x = -2; x <= 2; x++){
        for(int y = -2; y <= 2; y++){
            matrix[x+2][y+2] = exp(-1*(x*x + y*y)/(2*std*std));
        }
    }
    float sum = 0;
    for(int x = 0; x < 5; x++){
        for(int y = 0; y < 5; y++){
            sum += matrix[x][y];
        }
    }
    for(int x = 0; x < 5; x++){
        for(int y = 0; y < 5; y++){
            matrix[x][y] = matrix[x][y]/sum;
        }
    }
    LidaMatrix* mask = new LidaMatrix(5,5);
    for(int i = 0; i < mask->row; i++){
        for(int j = 0; j < mask->col; j++){
            mask->operator()(i,j) = matrix[i][j];
        }
    }
    LidaImage* unsharp = convolution(input, mask);
    LidaImage* g_mask = new LidaImage(input->row, input->col);
    for (int x = 0; x < g_mask->row; x++) {
        for (int y = 0; y < g_mask->col; y++) {
            g_mask->matrix3D[0][x][y] = (input->matrix3D[0][x][y]) - (unsharp->matrix3D[0][x][y]);
            g_mask->matrix3D[1][x][y] = (input->matrix3D[1][x][y]) - (unsharp->matrix3D[1][x][y]);
            g_mask->matrix3D[2][x][y] = (input->matrix3D[2][x][y]) - (unsharp->matrix3D[2][x][y]);
        }
    }

    LidaImage* output = new LidaImage(input->row, input->col);
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            output->matrix3D[0][x][y] = (input->matrix3D[0][x][y]) + k*(g_mask->matrix3D[0][x][y]);
            output->matrix3D[1][x][y] = (input->matrix3D[1][x][y]) + k*(g_mask->matrix3D[1][x][y]);
            output->matrix3D[2][x][y] = (input->matrix3D[2][x][y]) + k*(g_mask->matrix3D[2][x][y]);
        }
    }
    delete mask;
    delete unsharp;
    delete g_mask;
    return output;
}

LidaImage* SpatialFiltering::sobel(LidaImage *input)
{
    float sobel_x[3][3] = {{-1, -2, -1},
                          {0, 0, 0},
                          {1, 2, 1}};
    LidaMatrix* mask_x = new LidaMatrix(3,3);
    for(int i = 0; i < mask_x->row; i++){
        for(int j = 0; j < mask_x->col; j++){
            mask_x->operator()(i,j) = sobel_x[i][j];
        }
    }
    LidaImage* g_x = convolution(input, mask_x);
    float sobel_y[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};
    LidaMatrix* mask_y = new LidaMatrix(3,3);
    for(int i = 0; i < mask_y->row; i++){
        for(int j = 0; j < mask_y->col; j++){
            mask_y->operator()(i,j) = sobel_y[i][j];
        }
    }
    LidaImage* g_y = convolution(input, mask_y);
    LidaImage* output = new LidaImage(input->row, input->col);
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            float value = abs(g_x->matrix3D[0][x][y]) + abs(g_y->matrix3D[0][x][y]) + abs(g_x->matrix3D[1][x][y]) + abs(g_y->matrix3D[1][x][y]) + abs(g_x->matrix3D[2][x][y]) + abs(g_y->matrix3D[2][x][y]);
//            value = value / 3.0;
            output->matrix3D[0][x][y] = value;
            output->matrix3D[1][x][y] = value;
            output->matrix3D[2][x][y] = value;
//            output->matrix3D[0][x][y] = abs(g_x->matrix3D[0][x][y]) + abs(g_y->matrix3D[0][x][y]);
//            output->matrix3D[1][x][y] = abs(g_x->matrix3D[1][x][y]) + abs(g_y->matrix3D[1][x][y]);
//            output->matrix3D[2][x][y] = abs(g_x->matrix3D[2][x][y]) + abs(g_y->matrix3D[2][x][y]);
        }
    }
    delete mask_x;
    delete g_x;
    delete mask_y;
    delete g_y;
    return output;
}

LidaImage *SpatialFiltering::gradient_magnitude(LidaImage *input)
{
    float sobel_x[3][3] = {{-1, -2, -1},
                          {0, 0, 0},
                          {1, 2, 1}};
    LidaMatrix* mask_x = new LidaMatrix(3,3);
    for(int i = 0; i < mask_x->row; i++){
        for(int j = 0; j < mask_x->col; j++){
            mask_x->operator()(i,j) = sobel_x[i][j];
        }
    }
    LidaImage* g_x = convolution(input, mask_x);
    float sobel_y[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};
    LidaMatrix* mask_y = new LidaMatrix(3,3);
    for(int i = 0; i < mask_y->row; i++){
        for(int j = 0; j < mask_y->col; j++){
            mask_y->operator()(i,j) = sobel_y[i][j];
        }
    }
    LidaImage* g_y = convolution(input, mask_y);
    LidaImage* output = new LidaImage(input->row, input->col);
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            output->matrix3D[0][x][y] = std::sqrt(std::pow((g_x->matrix3D[0][x][y]),2) + std::pow(abs(g_y->matrix3D[0][x][y]),2));
            output->matrix3D[1][x][y] = std::sqrt(std::pow((g_x->matrix3D[1][x][y]),2) + std::pow(abs(g_y->matrix3D[1][x][y]),2));
            output->matrix3D[2][x][y] = std::sqrt(std::pow((g_x->matrix3D[2][x][y]),2) + std::pow(abs(g_y->matrix3D[2][x][y]),2));

            int average = (output->matrix3D[0][x][y] + output->matrix3D[1][x][y] + output->matrix3D[2][x][y])/3;
            output->matrix3D[0][x][y] = average;
            output->matrix3D[1][x][y] = average;
            output->matrix3D[2][x][y] = average;
        }
    }
    delete mask_x;
    delete g_x;
    delete mask_y;
    delete g_y;
    return output;
}

LidaImage *SpatialFiltering::gradient_angle(LidaImage *input)
{
    float sobel_x[3][3] = {{-1, -2, -1},
                          {0, 0, 0},
                          {1, 2, 1}};
    LidaMatrix* mask_x = new LidaMatrix(3,3);
    for(int i = 0; i < mask_x->row; i++){
        for(int j = 0; j < mask_x->col; j++){
            mask_x->operator()(i,j) = sobel_x[i][j];
        }
    }
    LidaImage* g_x = convolution(input, mask_x);
    float sobel_y[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};
    LidaMatrix* mask_y = new LidaMatrix(3,3);
    for(int i = 0; i < mask_y->row; i++){
        for(int j = 0; j < mask_y->col; j++){
            mask_y->operator()(i,j) = sobel_y[i][j];
        }
    }
    LidaImage* g_y = convolution(input, mask_y);
    for (int x = 0; x < input->row; x++) {
        for (int y = 0; y < input->col; y++) {
            float avg_g_x = (g_x->matrix3D[0][x][y] + g_x->matrix3D[1][x][y] + g_x->matrix3D[1][x][y])/3;
            float avg_g_y = (g_y->matrix3D[0][x][y] + g_y->matrix3D[1][x][y] + g_y->matrix3D[1][x][y])/3;

            for(int z = 0; z < 3; z++){
                g_x->matrix3D[z][x][y] = avg_g_x;
                g_y->matrix3D[z][x][y] = avg_g_y;
            }
        }
    }


    LidaImage* output = new LidaImage(input->row, input->col);
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            float a = atan2(-1,100) * 180/3.14; // y/x
            if(a < 0){
                a = a + 360;
            }
            output->matrix3D[0][x][y] = std::atan2(g_y->matrix3D[0][x][y], g_x->matrix3D[0][x][y])* 180/3.14;
            output->matrix3D[1][x][y] = std::atan2(g_y->matrix3D[1][x][y], g_x->matrix3D[1][x][y])* 180/3.14;
            output->matrix3D[2][x][y] = std::atan2(g_y->matrix3D[2][x][y], g_x->matrix3D[2][x][y])* 180/3.14;

            if(output->matrix3D[0][x][y] < 0){
                output->matrix3D[0][x][y] += 360;
            }
            if(output->matrix3D[1][x][y] < 0){
                output->matrix3D[1][x][y] += 360;
            }
            if(output->matrix3D[2][x][y] < 0){
                output->matrix3D[2][x][y] += 360;
            }
        }
    }
    delete mask_x;
    delete g_x;
    delete mask_y;
    delete g_y;
    return output;
}

LidaImage* SpatialFiltering::fuzzy(LidaImage *input)
{
    LidaImage* output = new LidaImage(input->row, input->col);
    float ZE[256];
    float BL[256];
    float WH[256];
    int b = 40;
    for (int i = 0; i < 256; i++) {
        if(i <= 80){
            ZE[i] = exp(-1.0*(i*i)/(2*b*b));
        }
        else {
            ZE[i] = 0;
        }
    }
    for (int i = 0; i < 256; i++) {
        if(i <= 191){
            BL[i] = (-1.0/191)*i+1;
        }
        else {
            BL[i] = 0;
        }
    }
    for (int i = 0; i < 256; i++) {
        if(i <= 64){
            WH[i] = 0;
        }
        else {
            WH[i] = (1.0/191)*(i-64);
        }
    }
//    for (int i = 0; i < 256; i++) {
//        cout << BL[i] << " " << WH[i] << endl;
//    }
    float lambda[4];
    float lambda_E = 0;
    float Q_if[4][256];
    float Q_else[256];
    float Q[256];
    for (int x = 0+1; x < (output->row-1); x++) {
        for (int y = 0+1; y < (output->col-1); y++) {
            for (int channel = 0; channel < 3; channel++) {
                int d2 = abs(input->matrix3D[channel][x-1][y] - input->matrix3D[channel][x][y]);
                int d4 = abs(input->matrix3D[channel][x][y-1] - input->matrix3D[channel][x][y]);
                int d6 = abs(input->matrix3D[channel][x][y+1] - input->matrix3D[channel][x][y]);
                int d8 = abs(input->matrix3D[channel][x+1][y] - input->matrix3D[channel][x][y]);
                float fuzzy_d2 = ZE[d2];
                float fuzzy_d4 = ZE[d4];
                float fuzzy_d6 = ZE[d6];
                float fuzzy_d8 = ZE[d8];
                if(fuzzy_d2 < fuzzy_d6){
                    lambda[0] = fuzzy_d2;
                }
                else {
                    lambda[0] = fuzzy_d6;
                }
                if(fuzzy_d6 < fuzzy_d8){
                    lambda[1] = fuzzy_d6;
                }
                else {
                    lambda[1] = fuzzy_d8;
                }
                if(fuzzy_d8 < fuzzy_d4){
                    lambda[2] = fuzzy_d8;
                }
                else {
                    lambda[2] = fuzzy_d4;
                }
                if(fuzzy_d4 < fuzzy_d2){
                    lambda[3] = fuzzy_d4;
                }
                else {
                    lambda[3] = fuzzy_d2;
                }
                for (int j = 0; j < 256; j++) {
                    for (int i = 0; i < 4; i++) {
                        if(WH[j] > lambda[i]){
                            Q_if[i][j] = lambda[i];
                        }
                        else {
                            Q_if[i][j] = WH[j];
                        }
                    }
                }
                lambda_E = lambda[0];
                for (int i = 0; i < 4; i++) {
                    if(lambda_E < lambda[i]){
                        lambda_E = lambda[i];
                    }
                }
                lambda_E = 1 - lambda_E;
                for (int j = 0; j < 256; j++) {
                    if(BL[j] > lambda_E){
                        Q_else[j] = lambda_E;
                    }
                    else {
                        Q_else[j] = BL[j];
                    }
                }
                for (int j = 0; j < 256; j++) {
                    Q[j] = Q_else[j];
                    for (int i = 0; i < 4; i++) {
                        if(Q_if[i][j] > Q[j]){
                            Q[j] = Q_if[i][j];
                        }
                    }
                }
                float numerator = 0;
                float denomirator = 0;
                for (int j = 0; j < 256; j++) {
                    numerator += (j*Q[j]);
                    denomirator += Q[j];
                }
                output->matrix3D[channel][x][y] = numerator/denomirator;
            }
        }
    }
    return output;
}

LidaImage *SpatialFiltering::edgeDetection(LidaImage *input)
{
    float sobel_x[3][3] = {{-1, -2, -1},
                          {0, 0, 0},
                          {1, 2, 1}};
    LidaMatrix* mask_x = new LidaMatrix(3,3);
    for(int i = 0; i < mask_x->row; i++){
        for(int j = 0; j < mask_x->col; j++){
            mask_x->operator()(i,j) = sobel_x[i][j];
        }
    }
    LidaImage* g_x = convolution(input, mask_x);
    float sobel_y[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};
    LidaMatrix* mask_y = new LidaMatrix(3,3);
    for(int i = 0; i < mask_y->row; i++){
        for(int j = 0; j < mask_y->col; j++){
            mask_y->operator()(i,j) = sobel_y[i][j];
        }
    }
    LidaImage* g_y = convolution(input, mask_y);
    LidaImage* output = new LidaImage(input->row, input->col);
    float gxx = 0;
    float gyy = 0;
    float gxy = 0;
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            gxx = std::pow(g_x->matrix3D[0][x][y],2) + std::pow(g_x->matrix3D[1][x][y],2) + std::pow(g_x->matrix3D[2][x][y],2);
            gyy = std::pow(g_y->matrix3D[0][x][y],2) + std::pow(g_y->matrix3D[1][x][y],2) + std::pow(g_y->matrix3D[2][x][y],2);
            gxy = (g_x->matrix3D[0][x][y])*(g_y->matrix3D[0][x][y]) + (g_x->matrix3D[1][x][y])*(g_y->matrix3D[1][x][y]) + (g_x->matrix3D[2][x][y])*(g_y->matrix3D[2][x][y]);

            float theta = 0.5*(std::atan(2*gxy/(gxx-gyy)));
            float F = std::sqrt(0.5*(gxx+gyy+(gxx-gyy)*(std::cos(2*theta))+2*gxy*(std::sin(2*theta))));
            theta += 3.1415926/2;
            float F2 = std::sqrt(0.5*(gxx+gyy+(gxx-gyy)*(std::cos(2*theta))+2*gxy*(std::sin(2*theta))));

//            cout << F << " ";
            if(F > F2){
                output->matrix3D[0][x][y] = F;
                output->matrix3D[1][x][y] = F;
                output->matrix3D[2][x][y] = F;
            }
            else {
                output->matrix3D[0][x][y] = F2;
                output->matrix3D[1][x][y] = F2;
                output->matrix3D[2][x][y] = F2;
            }

//            theta += 3.1415926/2;
//            F = std::sqrt(0.5*(gxx+gyy+(gxx-gyy)*(std::cos(2*theta))+2*gxy*(std::sin(2*theta))));
//            cout << F << endl;
        }
    }
    delete mask_x;
    delete g_x;
    delete mask_y;
    delete g_y;
    return output;
}
