#include "linearresampler.hpp"
#include <opencv2/highgui.hpp>

#include <QDebug>
#include <iostream>

nitro::LinearSampler::LinearSampler() = default;

nitro::LinearSampler::~LinearSampler() = default;

static inline float mix(float a, float b, float w) {
    return (1.0f - w) * a + w * b;
}

static inline float get(const cv::Mat &colorTable, int i) {
    return float(colorTable.at<uchar>(i, 0));
}

float nitro::LinearSampler::distFuncIndexed(const cv::Mat &colorTable,
                                            const std::vector<cv::Mat> &df,
                                            int x, int y, float p, int numLevelsInput) const {

    // find the index values that sit in between this value
    float oldGray = p * 255.0f;

    int layer0 = -1;

    for (int i = 0; i < numLevelsInput - 1; i++) {
        if (oldGray >= get(colorTable, i) && oldGray <= get(colorTable, i + 1)) {
            layer0 = i;
            break;
        }
    }
    if (layer0 == -1) {
        if (oldGray >= get(colorTable, numLevelsInput - 1)) {
            layer0 = numLevelsInput - 1;
        } else {
            layer0 = 0;
        }
    }

    int layer1 = std::min(layer0 + 1, numLevelsInput - 1);

    float t = (oldGray - get(colorTable, layer0)) / float(get(colorTable, layer1) - get(colorTable, layer0));


    float d1 = df[layer0].at<float>(y, x);
    float d2 = df[layer1].at<float>(y, x);

    return mix(d1, d2, t);
}
