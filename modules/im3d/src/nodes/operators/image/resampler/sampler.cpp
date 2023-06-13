#include "sampler.hpp"
#include <opencv2/highgui.hpp>
#include <QDebug>
#include <iostream>

nitro::Sampler::Sampler() = default;

nitro::Sampler::~Sampler() = default;


static inline float mix(float a, float b, float w) {
    return (1.0f - w) * a + w * b;
}

static inline float get(const cv::Mat &colorTable, int i) {
    return colorTable.at<float>(i, 0);
}

float nitro::Sampler::distFunc(const cv::Mat &colorTable,
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


    float p0 = df[layer0].at<float>(y, x);
    float p1 = df[layer1].at<float>(y, x);

    return mix(p0, p1, t);
}


cv::Mat nitro::Sampler::resample(const cv::Mat &img, const cv::Mat &colTable,
                                 const std::vector<cv::Mat> &df,
                                 int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled = cv::Mat::zeros(height, width, CV_32FC1);
    int numLevelsInput = df.size();

#pragma omp parallel for default(none) firstprivate(height, width, numDesiredLevels, numLevelsInput) shared(df, resampled, colTable, img)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int start = img.at<int>(y, x);
            for (int d = start; d >= 0; d--) {
                float p = float(d) / (float(numDesiredLevels) - 1.0f);
                float dist = distFunc(colTable, df, x, y, p, numLevelsInput);
                if (dist <= 0) {
                    resampled.at<float>(y, x) = p;
                    break;
                }
            }
        }
    }

    return resampled;
}
