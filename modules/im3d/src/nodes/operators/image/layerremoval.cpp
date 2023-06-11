#include "layerremoval.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>


#define EPSILON 0.00001

// Source: https://github.com/dennisjosesilva/morphotree

/**
 *
 * @param data The data
 * @param data_count row count of data
 * @param emi_peaks emission peaks will be put here
 * @param delta delta used for distinguishing peaks
 * @param emi_first should we search emission peak first of
                                     absorption peak first?
 */
static void
detect_peak(const std::vector<double> &data, int data_count, std::vector<int> &emi_peaks, double delta, int emi_first) {
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

static void find_peaks(std::vector<double> &importance, double width) {
    double impFac = 0.1;
    std::vector<int> v;
    int numIter = 0;
    while (numIter < 1000) {
        v.clear();
        detect_peak(importance, 256, v, impFac, 0);
        if (v.size() < width)
            impFac *= .9;
        else if (v.size() > width)
            impFac /= .9;
        else
            break;
        if (impFac < 0.0002) break;//Too small, then break
        numIter++;
    }
    std::fill(importance.begin(), importance.end(), 0);
    for (auto elem: v) {
        importance[elem] = 1;
    }
}

static void
detect_layers(int clear_color, std::vector<double> &upper_level, double threshold, bool needAssign, int &peaks,
              int max_elem) {

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
        difference = copy_upper_level[i + StartPoint] - copy_upper_level[i]; //attention: here shouldn't be upper_level
        if (difference > threshold) { //choose this layer
            if (needAssign) {
                upper_level[i + StartPoint] = 2; //Give it a num that it can never be.
            }

            i = i + StartPoint;
            StartPoint = distinguishable_interval;
            peaks++;
        } else {
            StartPoint += 1;
        }
    }

}

//binary search
static void
find_layers(int clear_color, std::vector<double> &importance_upper, double width, std::vector<double> &importance,
            int max_elem) {
    double impFac = 0.5;
    double head = 0;
    double tail = 0.5;
    int numIter = 0;
    int peaks = 0;
    while (numIter < 255) {
        if (impFac < 0.003) break;// The difference is too small
        detect_layers(clear_color, importance_upper, impFac, false, peaks, max_elem);
        if (peaks < width) { // impFac need a smaller one
            tail = impFac;
            impFac = (head + impFac) / 2;
        } else if (peaks > width) { // impFac need a bigger one
            head = impFac;
            impFac = (tail + impFac) / 2;
        } else
            break;
        numIter++;
    }

    detect_layers(clear_color, importance_upper, impFac, true, peaks, max_elem);//the impfac to be calculated is 0.003/2

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
static std::vector<double> calculateImportance(const cv::Mat &img, bool cumulative, int num_layers) {
    int min_elem = 1e5;
    int max_elem = 0;

    std::vector<double> importance;
    std::vector<double> upperLevelSet;
    importance.resize(256);
    upperLevelSet.resize(256);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int pixelValue = img.at<uchar>(i, j);
            min_elem = MIN(min_elem, pixelValue);
            max_elem = MAX(max_elem, pixelValue);
            importance[pixelValue]++; // histogram
        }
    }

    int clear_color = min_elem;
    upperLevelSet[0] = importance[0];
    for (int i = 0; i < 256; i++) {
        upperLevelSet[i + 1] = importance[i + 1] + upperLevelSet[i];
    }

    // Normalize. Local-maximal method.
    int normFac = int(*std::max_element(importance.begin(), importance.end()));
    for (int i = 0; i < 256; ++i) {
        importance[i] /= static_cast<double>(normFac);
    }
    // Normalize. Cumulative method.
    double max = upperLevelSet[256];
    for (int i = 0; i < 256; ++i) {
        upperLevelSet[i] = upperLevelSet[i] / static_cast<double>(max) - EPSILON;//To avoid to be 1.
    }

    // Cumulative method.
    if (cumulative) {
        find_layers(clear_color, upperLevelSet, num_layers, importance, max_elem);
    } else {
        // else local-maxima method
        find_peaks(importance, num_layers);
    }

    importance[clear_color] = 1;
    return importance;
}

static cv::Mat removeLayers(const cv::Mat &img, const std::vector<double> &importance) {
    cv::Mat res;
    img.copyTo(res);

    for (int y = 0; y < res.rows; y++) {
        for (int x = 0; x < res.cols; x++) {
            int pixelValue = img.at<uchar>(y, x);
            int val_up = pixelValue;
            int val_dn = pixelValue;
            if (importance[val_up] == 1)
                continue;
            while (val_dn >= 0 || val_up <= 256) {
                if (val_dn >= 0) {
                    if (importance[val_dn] == 1) {
                        res.at<uchar>(y, x) = val_dn;
                        break;
                    }
                }
                if (val_up <= 256) {
                    if (importance[val_up] == 1) {
                        res.at<uchar>(y, x) = val_up;
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

void nitro::LayerRemovalOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int k = nodePorts.getInputInteger("K", kPresent);
    auto img = nodePorts.getInputImage("Image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }

    cv::Mat res;
    bool cumulative = true;
    if (img->channels() == 1) {
        std::vector<double> importance = calculateImportance(*img, cumulative, k - 1);
        res = removeLayers(*img, importance);
    } else {
        std::vector<cv::Mat> channels;
        std::vector<cv::Mat> outChannels;
        cv::split(*img, channels);
        outChannels.resize(channels.size());
        for (int i = 0; i < channels.size(); i++) {
            auto channelImg = channels[i];
            std::vector<double> importance = calculateImportance(channelImg, cumulative, k - 1);
            outChannels[i] = removeLayers(channelImg, importance);
        }
        cv::merge(outChannels, res);
    }

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(res));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::LayerRemovalOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Layer Removal", "layerRemoval", category);
        return builder.withOperator(std::make_unique<nitro::LayerRemovalOperator>())->withIcon(
                ":/icons/nodes/quantize.png")->withNodeColor({43, 101, 43})->withInputImage("Image")->withInputInteger(
                "K", 8, 1, 255)->withOutputImage("Image")->build();
    };
}
