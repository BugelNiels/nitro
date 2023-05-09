#include "slr.hpp"

#include "core/algorithms/util/histogram.hpp"
#include <QtAlgorithms>
#include <QDebug>

#define EPSILON 0.00001

static void detect_peak(const double *data, /* the data */
                        int data_count, /* row count of data */
                        std::vector<int> &emi_peaks, /* emission peaks will be put here */
                        double delta, /* delta used for distinguishing peaks */
                        int emi_first /* should we search emission peak first of
                                     absorption peak first? */
) {
    int i;
    double mx;
    double mn;
    int mx_pos = 0;
    int mn_pos = 0;
    int is_detecting_emi = emi_first;


    mx = data[0];
    mn = data[0];

    for (i = 1; i < data_count; ++i) {
        if (data[i] > mx) {
            mx_pos = i;
            mx = data[i];
        }
        if (data[i] < mn) {
            mn_pos = i;
            mn = data[i];
        }

        if (is_detecting_emi && data[i] < mx - delta) {

            emi_peaks.push_back(mx_pos);
            is_detecting_emi = 0;

            i = mx_pos - 1;

            mn = data[mx_pos];
            mn_pos = mx_pos;
        } else if ((!is_detecting_emi) && data[i] > mn + delta) {

            is_detecting_emi = 1;

            i = mn_pos - 1;

            mx = data[mn_pos];
            mx_pos = mn_pos;
        }
    }
}

static void find_peaks(double *importance, double width) {
    double impfac = 0.1;
    std::vector<int> v;
    int numiters = 0;
    while (numiters < 1000) {
        v.clear();
        detect_peak(importance, 256, v, impfac, 0);
        if (v.size() < width)
            impfac *= .9;
        else if (v.size() > width)
            impfac /= .9;
        else
            break;
        if (impfac < 0.0002) break;//Too small, then break
        numiters++;
        //cout<<" "<<impfac;
    }
    memset(importance, 0, 256 * sizeof(double));
    for (auto elem: v)
        importance[elem] = 1;
}

static void
detect_layers(int clear_color, double *upper_level, double threshold, bool needAssign, int &peaks, int max_elem) {

    int distinguishable_interval = 1; //distinguishable_interval is set to 7
    peaks = 0;
    int i = clear_color;
    int StartPoint = distinguishable_interval; //There is no need to check i and i+1; it is not distinguished by eyes, so check between i and i+StartPoint.
    double difference;

    double copy_upper_level[256];

    for (int j = 0; j < 256; ++j) {
        copy_upper_level[j] = upper_level[j];
    }

    while ((i + StartPoint) < (max_elem + 1)) {
        difference = copy_upper_level[i + StartPoint] - copy_upper_level[i];//attention: here shouldn't be upper_level
        if (difference > threshold)//choose this layer
        {
            if (needAssign) {
                upper_level[i + StartPoint] = 2; //Give it a num that it can never be.
            }

            i = i + StartPoint;
            StartPoint = distinguishable_interval;
            peaks++;
        } else {
            StartPoint += 1;
        }
        //std::cout<<"Choose_L: "<<i<<" StartPoint: "<<StartPoint<<" peaks "<<peaks<<" diff: "<<difference<<" thres: "<<threshold<<endl;
    }

}

//binary search
static void find_layers(int clear_color, double *importance_upper, double width, double *importance, int max_elem) {
    //cout<<"width: "<<width<<endl;
    double impfac = 0.5;
    double head = 0;
    double tail = 0.5;
    int numiters = 0;
    int peaks = 0;
    while (numiters < 100) {
        if (impfac < 0.003) break;//The difference is too small
        detect_layers(clear_color, importance_upper, impfac, false, peaks, max_elem);
        if (peaks < width) {//impfac need a smaller one
            tail = impfac;
            impfac = (head + impfac) / 2;
        } else if (peaks > width) //impfac need a bigger one
        {
            head = impfac;
            impfac = (tail + impfac) / 2;
        } else
            break;
        numiters++;
    }

    detect_layers(clear_color, importance_upper, impfac, true, peaks, max_elem);//the impfac to be calculated is 0.003/2

    for (int i = 0; i < 256; ++i) {
        if (importance_upper[i] == 2) {
            importance[i] = 1;
        } else {
            importance_upper[i] = 0;
            importance[i] = 0;
        }
    }
}


/*
* Calculate the histogram of the image, which is equal to the importance for each level.
*/
double *calculateImportance(const nitro::CbdImage &img, bool cumulative, int num_layers, QVector<int>& layers) {
    //printf("Selecting %d layers...\n", num_layers);
    int normFac = 0;
//    float *c = processedImage->data();
//    float *end = processedImage->data() + nPix;

    int nPix = img.width() * img.height();

    const auto &imDat = img.constData();

    int min_elem = 1e5;
    int max_elem = 0;

    double *importance = static_cast<double *>(calloc(256, sizeof(double)));
    double *UpperLevelSet = static_cast<double *>(calloc(256, sizeof(double)));
    for (int i = 0; i < nPix; i++) {
        min_elem = MIN(min_elem, imDat.get(i));
        max_elem = MAX(max_elem, imDat.get(i));
        importance[imDat.get(i)]++; // histogram
    }

    int clear_color = min_elem;

    //cout<<" Create a histogram"<<endl;
    UpperLevelSet[0] = importance[0];
    for (int i = 0; i < 255; i++) {
        UpperLevelSet[i + 1] = importance[i + 1] + UpperLevelSet[i];
    }


    // Normalize. Local-maximal method.
    normFac = static_cast<int>(*std::max_element(importance, importance + 256));//find the max one.
    for (int i = 0; i < 256; ++i) {
        importance[i] /= static_cast<double>(normFac);
    }
    // Normalize. Cumulative method.
    double max = UpperLevelSet[255];
    for (int i = 0; i < 256; ++i) {
        UpperLevelSet[i] = UpperLevelSet[i] / static_cast<double>(max) - EPSILON;//To avoid to be 1.
    }

    // Cumulative method.
    if (cumulative) {
        find_layers(clear_color, UpperLevelSet, num_layers, importance, max_elem);
    } else {// else local-maxima method
        find_peaks(importance, num_layers);
    }

    importance[clear_color] = 1;

    for (int i = 0; i < 256; ++i) {
        if (importance[i] == 1) {
            layers.push_back(i);
        }
    }
    if (layers.size() == 0) {
        printf("ERROR: No layers selected. Exiting...\n");
        exit(-1);
    }
    printf("Selected %lu layers: ", layers.size());
    fflush(stdout);
    free(UpperLevelSet);
    return importance;
}

nitro::CbdImage removeLayers(const nitro::CbdImage &img, const double *importance) {
    nitro::CbdImage res(img);

    auto &data = res.data();
    for (int y = 0; y < res.height(); y++) {
        for (int x = 0; x < res.width(); x++) {
            int val_up = data.get(x, y);
            int val_dn = data.get(x, y);
            if (importance[val_up] == 1)
                continue;
            while (val_dn >= 0 || val_up <= 256) {
                if (val_dn >= 0) {
                    if (importance[val_dn] == 1) {
                        data.set(x, y, val_dn);
                        break;
                    }
                }
                if (val_up <= 256) {
                    if (importance[val_up] == 1) {
                        data.set(x, y, val_up);
                        break;
                    }
                }
                val_dn--;
                val_up++;
            }
        }
    }
    return res;
}

nitro::CbdImage nitro::operations::smartLayerRemoval(const nitro::CbdImage &img, int k) {

    bool cumulative = false;
    QVector<int> layers;
    double *importance = calculateImportance(img, cumulative, k, layers);
    auto res = removeLayers(img, importance);

    auto &data = res.data();
    for (int y = 0; y < res.height(); y++) {
        for (int x = 0; x < res.width(); x++) {
            int i;
            int val = data.get(x, y);
            for(i = 0; i < layers.size(); i++) {
                if(val == layers[i]) {
                    break;
                }
            }
            data.set(x, y, i);
        }
    }
    res.setIndexed(layers);
    return res;
}