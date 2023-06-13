#include "linearsampler.hpp"
#include <opencv2/highgui.hpp>

#include <iostream>

nitro::LinearSampler::LinearSampler() = default;

nitro::LinearSampler::~LinearSampler() = default;

static inline float mix(float a, float b, float w) {
    return (1.0f - w) * a + w * b;
}

static inline float get(const cv::Mat &colorTable, int i) {
    return colorTable.at<float>(i, 0);
}

float nitro::LinearSampler::distFuncIndexed(const cv::Mat &colorTable,
                                            const std::vector<cv::Mat> &df,
                                            int x, int y, float p, int numLevelsInput) const {


    int layer0 = -1;

    // find the index values that sit in between this value
    for (int i = 0; i < numLevelsInput - 1; i++) {
        if (p >= get(colorTable, i) && p <= get(colorTable, i + 1)) {
            layer0 = i;
            break;
        }
    }
    if (layer0 == -1) {
        if (p >= get(colorTable, numLevelsInput - 1)) {
            layer0 = numLevelsInput - 1;
        } else {
            layer0 = 0;
        }
    }

    int layer1 = std::min(layer0 + 1, numLevelsInput - 1);

    float layer0Col = get(colorTable, layer0);
    float layer1Col = get(colorTable, layer1);

    float delta = (layer1Col - layer0Col);
    float t = (p - layer0Col) / delta;
    t = std::clamp(t, 0.0f, 1.0f);


    float p0 = df[layer0].at<float>(y, x);
    float p1 = df[layer1].at<float>(y, x);

    return mix(p0, p1, t);
}
