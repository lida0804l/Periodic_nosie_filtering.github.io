#include "panorama.h"

Panorama::Panorama()
{

}

LidaImage *Panorama::warping(LidaImage *input, float focal_length)
{
    // Background to white
//    LidaImage* output = new LidaImage(input->row, input->col);
//    for (int x = 0; x < output->row; x++) {
//        for (int y = 0; y < output->col; y++) {
//            output->matrix3D[0][x][y] = 255;
//            output->matrix3D[1][x][y] = 255;
//            output->matrix3D[2][x][y] = 255;
//        }
//    }



    LidaImage* output = new LidaImage(input->row, input->col);
    int mid_x = input->col/2;
    int mid_y = input->row/2;
    for (int x = 0; x < output->row; x++) {
        for (int y = 0; y < output->col; y++) {
            double theta = atan((y-mid_x)/focal_length);
            double h = (x-mid_y)/pow(pow((y-mid_x),2)+pow(focal_length,2),0.5);
            int x1 = focal_length*theta+mid_x;
            int y1 = focal_length*h+mid_y;
            output->matrix3D[0][y1][x1] = input->matrix3D[0][x][y];
            output->matrix3D[1][y1][x1] = input->matrix3D[1][x][y];
            output->matrix3D[2][y1][x1] = input->matrix3D[2][x][y];
        }
    }
    return output;
}

LidaImage *Panorama::harris_corner(LidaImage *input, std::vector<Descriptor>& features)
{
     LidaImage* output;
     LidaImage* gray = input->grayscale();
     LidaImage* I = SpatialFiltering::gaussian(gray);
     float sobel_x[3][3] = {{-1, -2, -1},
                           {0, 0, 0},
                           {1, 2, 1}};
     LidaMatrix* mask_x = new LidaMatrix(3,3);
     for(int i = 0; i < mask_x->row; i++){
         for(int j = 0; j < mask_x->col; j++){
             mask_x->operator()(i,j) = sobel_x[i][j];
         }
     }
     LidaImage* Ix = SpatialFiltering::convolution(I, mask_x);
     float sobel_y[3][3] = {{-1, 0, 1},
                           {-2, 0, 2},
                           {-1, 0, 1}};
     LidaMatrix* mask_y = new LidaMatrix(3,3);
     for(int i = 0; i < mask_y->row; i++){
         for(int j = 0; j < mask_y->col; j++){
             mask_y->operator()(i,j) = sobel_y[i][j];
         }
     }
     LidaImage* Iy = SpatialFiltering::convolution(I, mask_y);

     LidaImage* Ix_2 = new LidaImage(input->row, input->col);
     LidaImage* Iy_2 = new LidaImage(input->row, input->col);
     LidaImage* Ixy = new LidaImage(input->row, input->col);
     for (int x = 0; x < input->row; x++) {
         for (int y = 0; y < input->col; y++) {
             for(int channel = 0; channel < 3; channel++){
                 Ixy->matrix3D[channel][x][y] = (Ix->matrix3D[channel][x][y]) * (Iy->matrix3D[channel][x][y]);
                 Ix_2->matrix3D[channel][x][y] = (Ix->matrix3D[channel][x][y]) * (Ix->matrix3D[channel][x][y]);
                 Iy_2->matrix3D[channel][x][y] = (Iy->matrix3D[channel][x][y]) * (Iy->matrix3D[channel][x][y]);
             }
         }
     }
     LidaImage* Sx2 = SpatialFiltering::gaussian(Ix_2);
     LidaImage* Sy2 = SpatialFiltering::gaussian(Iy_2);
     LidaImage* Sxy = SpatialFiltering::gaussian(Ixy);
     LidaImage* R = new LidaImage(input->row, input->col);
     float k = 0.05; //0.05~0.5 most commonly seen
     for (int x = 0; x < input->row; x++) {
         for (int y = 0; y < input->col; y++) {
             for(int channel = 0; channel < 3; channel++){
                 float detM = (Sx2->matrix3D[channel][x][y])*(Sy2->matrix3D[channel][x][y]) - (Sxy->matrix3D[channel][x][y])*(Sxy->matrix3D[channel][x][y]);
                 float traceM = Sx2->matrix3D[channel][x][y] + Sy2->matrix3D[channel][x][y];
                 R->matrix3D[channel][x][y] = detM - k*traceM*traceM;
             }
         }
     }
     float maximum = 0;
     for (int x = 0; x < input->row; x++) {
         for (int y = 0; y < input->col; y++) {
             for(int channel = 0; channel < 3; channel++){
                 if(maximum < R->matrix3D[channel][x][y]){
                     maximum = R->matrix3D[channel][x][y];
                 }
             }
         }
     }
     float threshold = maximum*(0.001); // 1e+8  ps: maximum usually e+10
     for (int x = 0; x < input->row; x++) {
         for (int y = 0; y < input->col; y++) {
             for(int channel = 0; channel < 3; channel++){
                 if(R->matrix3D[channel][x][y] < threshold){
                     R->matrix3D[channel][x][y] = 0;
                 }
             }
         }
     }
     output = SpatialFiltering::non_maximum_supression(R);

     LidaImage* magnitude = SpatialFiltering::gradient_magnitude(input);
     LidaImage* angle = SpatialFiltering::gradient_angle(input);
     float weight[16][16];
     for (int x = 0; x < 16; x++) {
         for (int y = 0; y < 16; y++) {
             float D2 = (-7.5+x)*(-7.5+x) + (-7.5+y)*(-7.5+y);
             weight[x][y] = std::exp(-1*D2/(2*2.4*2.4));
         }
     }

     int feature_amount = 0;
     int count = 0;
     for (int x = 0; x < output->row; x++) {
         for (int y = 0; y < output->col; y++) {
             if(output->matrix3D[0][x][y] > 0){
                 if((x > 15) && (x < (input->row - 15)) && (y > 15) && (y < (input->col - 15))){
                     feature_amount++;
                     QVector<float> histogram(10);
                     QVector<float> histogram_sorted(10);
                     for (int i = 0; i < histogram.size(); i++) {
                         histogram[i] = 0;
                         histogram_sorted[i] = 0;
                     }
                     int idx_i = 0;
                     int idx_j = 0;
                     for (int i = -8; i < 8; i++) {
                         for (int j = -8; j < 8; j++) {
                             int bins = int(angle->matrix3D[0][x+i][y+j])/36;
                             histogram[bins] += weight[idx_i][idx_j]*magnitude->matrix3D[0][x+i][y+j];
                             histogram_sorted[bins] = histogram[bins];
                             idx_j++;
                         }
                         idx_j = 0;
                         idx_i++;
                     }
                     qSort(histogram_sorted.begin(), histogram_sorted.end(), qGreater<float>());

                     Descriptor a;
                     a.x = x;
                     a.y = y;
                     idx_i = 0;
                     idx_j = 0;
                     float theta = (histogram.indexOf(histogram_sorted.at(0)))*36 + 18;
                     for (int i = -8; i < 8; i++) {
                         for (int j = -8; j < 8; j++) {
                             int x_rotate = std::round(i*std::cos(theta) - j*std::sin(theta));
                             int y_rotate = std::round(i*std::sin(theta) + j*std::cos(theta));
                             int bins = int(angle->matrix3D[0][x+x_rotate][y+y_rotate] - theta);
                             if(bins < 0){
                                 bins += 360;
                             }
                             bins = bins/45;
                             a.hist[(idx_i/4)*32 + (idx_j/4)*8 + bins] += weight[idx_i][idx_j]*magnitude->matrix3D[0][x+x_rotate][y+y_rotate];
                             idx_j++;
                         }
                         idx_j = 0;
                         idx_i++;
                     }
                     float max = 0;
                     for (int i = 0; i < 128; i++) {
                         if(max < a.hist[i]){
                             max = a.hist[i];
                         }
                     }
                     for (int i = 0; i < 128; i++) {
                         a.hist[i] = (a.hist[i])/max;
                     }
                     max = 0;
                     for (int i = 0; i < 128; i++) {
                         if(a.hist[i] > 0.2){
                             a.hist[i] = 0.2;
                         }
                         if(max < a.hist[i]){
                             max = a.hist[i];
                         }
                     }
                     for (int i = 0; i < 128; i++) {
                         a.hist[i] = (a.hist[i])/max;
                     }
                     features.push_back(a);
                     if(histogram_sorted[1] > (histogram_sorted[0]*0.8)){
                         Descriptor b;
                         b.x = x;
                         b.y = y;
                         idx_i = 0;
                         idx_j = 0;
                         float theta = (histogram.indexOf(histogram_sorted.at(1)))*36 + 18;
                         for (int i = -8; i < 8; i++) {
                             for (int j = -8; j < 8; j++) {
                                 int x_rotate = std::round(i*std::cos(theta) - j*std::sin(theta));
                                 int y_rotate = std::round(i*std::sin(theta) + j*std::cos(theta));
                                 int bins = int(angle->matrix3D[0][x+x_rotate][y+y_rotate] - theta);
                                 if(bins < 0){
                                     bins += 360;
                                 }
                                 bins = bins/45;
                                 b.hist[(idx_i/4)*32 + (idx_j/4)*8 + bins] += weight[idx_i][idx_j]*magnitude->matrix3D[0][x+x_rotate][y+y_rotate];
                                 idx_j++;
                             }
                             idx_j = 0;
                             idx_i++;
                         }
                         float max = 0;
                         for (int i = 0; i < 128; i++) {
                             if(max < b.hist[i]){
                                 max = b.hist[i];
                             }
                         }
                         for (int i = 0; i < 128; i++) {
                             b.hist[i] = (b.hist[i])/max;
                         }
                         max = 0;
                         for (int i = 0; i < 128; i++) {
                             if(b.hist[i] > 0.2){
                                 b.hist[i] = 0.2;
                             }
                             if(max < b.hist[i]){
                                 max = b.hist[i];
                             }
                         }
                         for (int i = 0; i < 128; i++) {
                             b.hist[i] = (b.hist[i])/max;
                         }
                         features.push_back(b);
                     }
                 }
             }
         }
     }

     delete gray;
     delete I;
     delete mask_x;
     delete Ix;
     delete mask_y;
     delete Iy;
     delete Ix_2;
     delete Iy_2;
     delete Ixy;
     delete Sx2;
     delete Sy2;
     delete Sxy;
     delete R;
     delete magnitude;
     delete angle;
     return output;
}

Panorama::Match_pair Panorama::comparison(std::vector<Panorama::Descriptor> &features_1, std::vector<Panorama::Descriptor> &features_2)
{
    Panorama::Match_pair match_pair;
    for (int idx_1 = 0; idx_1 < features_1.size(); idx_1++) {
        int match_index_feature2;
        float smallest_distance;
        for (int idx_2 = 0; idx_2 < features_2.size(); idx_2++) {
            float D = 0;
            for (int k = 0; k < 128; k++) {
                D += std::pow(features_1[idx_1].hist[k] - features_2[idx_2].hist[k],2);
            }
            D = std::sqrt(D);
            if(idx_2 == 0){
                match_index_feature2 = idx_2;
                smallest_distance = D;
            }
            else {
                if(smallest_distance > D){
                    match_index_feature2 = idx_2;
                    smallest_distance = D;
                }
            }
        }
        if(smallest_distance < 1.5){ //Emperical
            match_pair.left.push_back(features_1[idx_1]);
            match_pair.right.push_back(features_2[match_index_feature2]);
        }
//        cout << smallest_distance << endl;
    }
    return match_pair;
}

void Panorama::outlier_removal(LidaImage *left_image, LidaImage *right_image, Panorama::Match_pair &match_pair)
{
    int remove_fraction = 4;
    for (int i = 0; i < (match_pair.left.size()-1); i++) {
        for (int j = 1; j < match_pair.left.size(); j++) {
            float slope = float(match_pair.right[j].x - match_pair.left[j].x) / (match_pair.right[j].y + (left_image->col - match_pair.left[j].y));
            float slope_previous = float(match_pair.right[j-1].x - match_pair.left[j-1].x) / (match_pair.right[j-1].y + (left_image->col - match_pair.left[j-1].y));
            if(slope < slope_previous){
                Descriptor tmp_left = match_pair.left[j];
                match_pair.left[j] = match_pair.left[j-1];
                match_pair.left[j-1] = tmp_left;
                Descriptor tmp_right = match_pair.right[j];
                match_pair.right[j] = match_pair.right[j-1];
                match_pair.right[j-1] = tmp_right;
            }
        }
    }
    int amount_remove = float(match_pair.left.size())/remove_fraction;
    while (amount_remove > 0) {
        match_pair.left.erase(match_pair.left.begin());
        match_pair.left.erase(match_pair.left.end()-1);
        match_pair.right.erase(match_pair.right.begin());
        match_pair.right.erase(match_pair.right.end()-1);
        amount_remove--;
    }

    for (int i = 0; i < (match_pair.left.size()-1); i++) {
        for (int j = 1; j < match_pair.left.size(); j++) {
            float length = std::sqrt(std::pow(match_pair.right[j].x - match_pair.left[j].x,2) + std::pow(match_pair.right[j].y + (left_image->col - match_pair.left[j].y),2));
            float length_previous = std::sqrt(std::pow(match_pair.right[j-1].x - match_pair.left[j-1].x,2) + std::pow(match_pair.right[j-1].y + (left_image->col - match_pair.left[j-1].y),2));
            if(length < length_previous){
                Descriptor tmp_left = match_pair.left[j];
                match_pair.left[j] = match_pair.left[j-1];
                match_pair.left[j-1] = tmp_left;
                Descriptor tmp_right = match_pair.right[j];
                match_pair.right[j] = match_pair.right[j-1];
                match_pair.right[j-1] = tmp_right;
            }
        }
    }
    amount_remove = float(match_pair.left.size())/remove_fraction;
    while (amount_remove > 0) {
        match_pair.left.erase(match_pair.left.begin());
        match_pair.left.erase(match_pair.left.end()-1);
        match_pair.right.erase(match_pair.right.begin());
        match_pair.right.erase(match_pair.right.end()-1);
        amount_remove--;
    }
}

QImage *Panorama::draw_feature_match(LidaImage *input1, LidaImage *input2, Panorama::Match_pair match_pair)
{
    int row_max = input1->row;
    if(input1->row < input2->row){
        row_max = input2->row;
    }
    LidaImage* stitch_image = new LidaImage(row_max, input1->col + input2->col);
    for (int x = 0; x < input1->row; x++) {
        for (int y = 0; y < input1->col; y++) {
            for (int channel = 0; channel < 3; channel++) {
                stitch_image->matrix3D[channel][x][y] = input1->matrix3D[channel][x][y];
            }
        }
    }
    for (int x = 0; x < input2->row; x++) {
        for (int y = 0; y < input2->col; y++) {
            for (int channel = 0; channel < 3; channel++) {
                stitch_image->matrix3D[channel][x][y+input1->col] = input2->matrix3D[channel][x][y];
            }
        }
    }
    QImage* output = new QImage(stitch_image->col, stitch_image->row, QImage::Format_RGB888);
    for (int x = 0; x < output->height(); x++) {
        for (int y = 0; y < output->width(); y++) {
            int r = stitch_image->matrix3D[0][x][y];
            int g = stitch_image->matrix3D[1][x][y];
            int b = stitch_image->matrix3D[2][x][y];
            output->setPixel(y,x,qRgb(r,g,b));
        }
    }
    QPainter p(output);
    srand (time(NULL));
    for (int i = 0; i < match_pair.left.size(); i++) {
        p.setPen(QPen(QColor(rand()%256,rand()%256,rand()%256), 2, Qt::DashDotLine, Qt::RoundCap));
        p.drawLine(match_pair.left[i].y,match_pair.left[i].x,match_pair.right[i].y + input1->col,match_pair.right[i].x);
    }
    delete stitch_image;
    return output;
}

LidaImage *Panorama::auto_stitch()
{
    LidaImage* output = new LidaImage();

    string Filepath;
    QStringList Filepaths;
    QFileDialog dialog;
    dialog.setWindowTitle("Choose the panorama components");
//    dialog.setDirectory("E:/EEM");
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
        image_sets[i] = Panorama::QImage2LidaImage(image);
        delete image;
    }

    std::vector<std::vector<Descriptor>> feature_sets(image_sets.size());
    for(int i = 0; i < Filepaths.size(); i++){
        LidaImage* corner_img = Panorama::harris_corner(image_sets[i], feature_sets[i]);
        delete corner_img;
    }
//    for(int i = 0; i < feature_sets[0].size(); i++){
//        cout << feature_sets[0][i].x << " " <<feature_sets[0][i].y << endl;
//        for (int k = 0; k < 128; k++) {
//            cout << feature_sets[0][i].hist[k] << " ";
//        }
//        cout << endl << endl;
//    }
    std::vector<LidaImage*> ordered_image_sets;
    std::vector<std::vector<Descriptor>> ordered_feature_sets;
    ordered_image_sets.insert(ordered_image_sets.begin(), image_sets[0]); //Test starting index at 3 or else
    image_sets.erase(image_sets.begin()+0);
    ordered_feature_sets.insert(ordered_feature_sets.begin(), feature_sets[0]);
    feature_sets.erase(feature_sets.begin()+0);
//    cout << ordered_image_sets.size() << " " << image_sets.size() << endl;
//    cout << ordered_feature_sets.size() << " " << feature_sets.size() << endl;
    int idx;
    int max_amount_match;
    for (int i = 0; i < image_sets.size(); i++) {
        if(i == 0){
            idx = i;
            max_amount_match = Panorama::comparison(ordered_feature_sets[0],feature_sets[i]).left.size();
        }
        else {
            int amount_match = Panorama::comparison(ordered_feature_sets[0],feature_sets[i]).left.size();
            if(max_amount_match < amount_match){
                idx = i;
                max_amount_match = amount_match;
            }
        }
    }
//    cout << idx << endl;
    Match_pair tmp_match_pair = Panorama::comparison(ordered_feature_sets[0],feature_sets[idx]);
    int* y_coordinate = new int[tmp_match_pair.left.size()];
    for (int i = 0; i < tmp_match_pair.left.size(); i++) {
        y_coordinate[i] = tmp_match_pair.left[i].y;
    }
    std::sort(y_coordinate,y_coordinate+tmp_match_pair.left.size());
    delete[] y_coordinate;
    if(y_coordinate[tmp_match_pair.left.size() / 2] > (ordered_image_sets[0]->col/2)){
//        cout << y_coordinate[tmp_match_pair.left.size() / 2] << " " << ordered_image_sets[0]->col/2 << endl;
        ordered_image_sets.push_back(image_sets[idx]);
        image_sets.erase(image_sets.begin()+idx);

        ordered_feature_sets.push_back(feature_sets[idx]);
        feature_sets.erase(feature_sets.begin()+idx);
    }
    else {
        ordered_image_sets.insert(ordered_image_sets.begin(), image_sets[idx]);
        image_sets.erase(image_sets.begin()+idx);

        ordered_feature_sets.insert(ordered_feature_sets.begin(), feature_sets[idx]);
        feature_sets.erase(feature_sets.begin()+idx);
    }
    cout << ordered_image_sets.size() << " " << image_sets.size() << endl;
    cout << ordered_feature_sets.size() << " " << feature_sets.size() << endl;


//    Match_pair left;
//    Match_pair right;
    while (image_sets.size() > 0) {
        cout << image_sets.size() << endl;
        int idx_left_match;
        int max_amount_left_match = 0;
        int idx_right_match;
        int max_amount_right_match = 0;
//        Match_pair left;
//        Match_pair right;
        for (int i = 0; i < image_sets.size(); i++) {
            if(i == 0){
                idx_left_match = 0;
                max_amount_left_match = Panorama::comparison(ordered_feature_sets[0],feature_sets[i]).left.size();
                idx_right_match = 0;
                max_amount_right_match = Panorama::comparison(ordered_feature_sets[ordered_feature_sets.size()-1],feature_sets[i]).left.size();
            }
            else {
                Match_pair tmp_left = Panorama::comparison(ordered_feature_sets[0],feature_sets[i]);
                Match_pair tmp_right = Panorama::comparison(ordered_feature_sets[ordered_feature_sets.size()-1],feature_sets[i]);
                int left_match = tmp_left.left.size();
                int right_match = tmp_right.left.size();
                if(max_amount_left_match < left_match){
                    idx_left_match = i;
                    max_amount_left_match = left_match;
//                    left = tmp_left;
                }
                if(max_amount_right_match < right_match){
                    idx_right_match = i;
                    max_amount_right_match = right_match;
//                    right = tmp_right;
                }
            }
        }
        cout << "run here " << max_amount_left_match << " " << max_amount_right_match << endl;
        Match_pair left = Panorama::comparison(ordered_feature_sets[0],feature_sets[idx_left_match]);
        Match_pair right = Panorama::comparison(ordered_feature_sets[ordered_feature_sets.size()-1],feature_sets[idx_right_match]);
        cout << "run here " << left.left.size() << " " << right.left.size() << endl;
        float ratio_correct_match_right = 0;
        float ratio_correct_match_left = 0;
        for (int i = 0; i < right.left.size(); i++) {
            if(right.left[i].y > (ordered_image_sets[ordered_image_sets.size()-1]->col/2)){
                ratio_correct_match_right++;
            }
        }
        ratio_correct_match_right /= right.left.size();
        for (int i = 0; i < left.left.size(); i++) {
            if(left.left[i].y < (ordered_image_sets[0]->col/2)){
                ratio_correct_match_left++;
            }
        }
        ratio_correct_match_left /= left.left.size();
        cout << ratio_correct_match_left << " " << ratio_correct_match_right << endl;

        if(ratio_correct_match_right > ratio_correct_match_left){
            ordered_image_sets.push_back(image_sets[idx_right_match]);
            image_sets.erase(image_sets.begin()+idx_right_match);

            ordered_feature_sets.push_back(feature_sets[idx_right_match]);
            feature_sets.erase(feature_sets.begin()+idx_right_match);
            cout << "right" << endl;
        }
        else {
            ordered_image_sets.insert(ordered_image_sets.begin(), image_sets[idx_left_match]);
            image_sets.erase(image_sets.begin()+idx_left_match);

            ordered_feature_sets.insert(ordered_feature_sets.begin(), feature_sets[idx_left_match]);
            feature_sets.erase(feature_sets.begin()+idx_left_match);
            cout << "left" << endl;
        }
    }
    return ordered_image_sets[0];
}

LidaImage *Panorama::stitch()
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
        image_sets[i] = Panorama::QImage2LidaImage(image);
        delete image;
    }
    std::vector<std::vector<Descriptor>> feature_sets(image_sets.size());
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


    for (int i = 0; i < image_sets.size(); i++) {
        delete image_sets[i];
    }
    return pano;
}

LidaImage *Panorama::QImage2LidaImage(QImage *input)
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

QImage *Panorama::LidaImage2QImage(LidaImage *input)
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
