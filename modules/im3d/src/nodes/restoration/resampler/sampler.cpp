#include "sampler.hpp"
#include <opencv2/highgui.hpp>
#include <QDebug>
#include <iostream>

nitro::Sampler::Sampler() = default;

nitro::Sampler::~Sampler() = default;


float nitro::Sampler::mix(float a, float b, float w) const {
    return (1.0f - w) * a + w * b;
}


cv::Mat nitro::Sampler::resample(const cv::Mat &img, const std::vector<float> &colTable,
                                 const std::vector<cv::Mat> &df,
                                 int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled = cv::Mat::zeros(height, width, CV_32FC1);

#pragma omp parallel for default(none) firstprivate(height, width, numDesiredLevels) shared(df, resampled, colTable, img)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            float val = img.at<float>(y, x);
            int start = numDesiredLevels - 1;

            float layer0Col, layer1Col;
            float p0, p1;
            for (int i = 0; i < colTable.size() - 1; ++i) {
                layer0Col = colTable[i];
                if (val == layer0Col) {
                    layer1Col = colTable[i + 1];
                    start = layer1Col * (numDesiredLevels - 1.0f);
                    p0 = df[i].at<float>(y, x);
                    p1 = df[i + 1].at<float>(y, x);
                    break;
                }
            }
            float delta = (layer1Col - layer0Col);


            for (int d = start; d >= 0; d--) {
                float p = float(d) / (float(numDesiredLevels) - 1.0f);
                float t = (p - layer0Col) / delta;
                if (mix(p0, p1, t) <= 0) {
                    resampled.at<float>(y, x) = p;
                    break;
                }
            }
        }
    }

    return resampled;
}
