#include "cubicsampler.hpp"

#include "spline.h"

#include <iostream>
#include <QDebug>

nitro::CubicSampler::CubicSampler() = default;

nitro::CubicSampler::~CubicSampler() = default;

static inline float get(const cv::Mat &colorTable, int i) {
    return colorTable.at<float>(i, 0);
}

static std::vector<double> colTableToVector(const cv::Mat &colorTable) {
    std::vector<double> colorTableVector;
    int numRows = colorTable.rows;
    colorTableVector.resize(numRows);

    for (int i = 0; i < numRows; ++i) {
        float colorValue = colorTable.at<float>(i, 0);
        colorTableVector[i] = static_cast<double>(colorValue);
    }

    return colorTableVector;
}

cv::Mat nitro::CubicSampler::resample(const cv::Mat &colTable,
                                      const std::vector<cv::Mat> &df,
                                      int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled(height, width, CV_32FC1);

    auto cols = colTableToVector(colTable);
    int numLevelsInput = df.size();


#pragma omp parallel default(none) firstprivate(height, width, numDesiredLevels, numLevelsInput) shared(df, resampled, cols)
    {
        std::vector<double> values;
        values.resize(numLevelsInput);
#pragma omp for
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int i = 0; i < numLevelsInput; ++i) {
                    values[i] = static_cast<double>(df[i].at<float>(y, x));
                }
                // TODO: fix the end point tangents
                tk::spline s(cols, values, tk::spline::cspline, true);
                // Compute interpolation tangents
                for (int d = numDesiredLevels - 1; d >= 0; d--) {
                    double p = double(d) / (double(numDesiredLevels) - 1.0);
                    double dist = s(p);
                    if (dist <= 0) {
                        resampled.at<float>(y, x) = float(p);
                        break;
                    }
                }
            }
        }
    }
    return resampled;
}
