#include "cubicsampler.hpp"

#include "spline.h"

#include <iostream>
#include <QDebug>

nitro::CubicSampler::CubicSampler() = default;

nitro::CubicSampler::~CubicSampler() = default;

cv::Mat nitro::CubicSampler::resample(const cv::Mat &img, const std::vector<float> &colTable,
                                      const std::vector<cv::Mat> &df,
                                      int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled = cv::Mat::zeros(height, width, CV_32FC1);
    int numLevelsInput = df.size();
    std::vector<double> cols(std::begin(colTable), std::end(colTable));

#pragma omp parallel default(none) firstprivate(height, width, numDesiredLevels, numLevelsInput) shared(df, resampled, cols, img)
    {
        std::vector<double> values;
        values.resize(numLevelsInput);
#pragma omp for
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int i = 0; i < numLevelsInput; ++i) {
                    values[i] = static_cast<double>(df[i].at<float>(y, x));
                }

                float val = img.at<float>(y, x);
                int start = numDesiredLevels - 1;
                for (int i = 0; i < cols.size() - 1; ++i) {
                    if (val == cols[i]) {
                        start = cols[i + 1] * (numDesiredLevels - 1.0f);
                        break;
                    }
                }
                // TODO: fix the end point tangents
                tk::spline s(cols, values, tk::spline::cspline, true);
                // Compute interpolation tangents
                for (int d = start; d >= 0; d--) {
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
