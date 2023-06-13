#include "sampler.hpp"
#include <opencv2/highgui.hpp>
#include <QDebug>
#include <iostream>

nitro::Sampler::Sampler() = default;

nitro::Sampler::~Sampler() = default;

cv::Mat nitro::Sampler::resample(const cv::Mat &colTable,
                                 const std::vector<cv::Mat> &df,
                                 int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled(height, width, CV_32FC1);
    int numLevelsInput = df.size();

#pragma omp parallel for default(none) firstprivate(height, width, numDesiredLevels, numLevelsInput) shared(df, resampled, colTable)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int d = numDesiredLevels - 1; d >= 0; d--) {
                float p = float(d) / (float(numDesiredLevels) - 1.0f);
                float dist = distFuncIndexed(colTable, df, x, y, p, numLevelsInput);
                if (dist <= 0) {
                    resampled.at<float>(y, x) = p;
                    break;
                }
            }
        }
    }

    return resampled;
}
