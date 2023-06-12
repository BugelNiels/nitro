#include "cubicinterpolatoryresampler.hpp"

#include <QDebug>

nitro::CubicInterpolatorySampler::CubicInterpolatorySampler() {}

nitro::CubicInterpolatorySampler::~CubicInterpolatorySampler() {}

static inline float get(const cv::Mat &colorTable, int i) {
    return float(colorTable.at<uchar>(i, 0));
}

/**
 * @brief CubicInterpolatorySampler::computeMonotonicTangents Computes the
 * monotinic tangents required for the Fitsch-Carlson method here.
 * @param distanceField Collection of distance fields from which to calculate
 * the tangents. Only calculates it for 1 single coordinate.
 * @param x The x coordinate in the distance field.
 * @param y The y coordinate in the distance field.
 * @param n The number of data points to calculate tangents for.
 */
void nitro::CubicInterpolatorySampler::computeMonotonicTangents(const cv::Mat &colorTable,
                                                                const std::vector<cv::Mat> &df,
                                                                int x, int y,
                                                                std::vector<float> &tangents,
                                                                std::vector<float> &secants) {
    int n = df.size();
    // This performs the first step of averaging the tangents
    for (int k = 0; k < n - 1; k++) {
        // index k for tangent computation
        int layer0 = k;
        // index k + 1 for tangent computation
        int layer1 = k + 1;  // std::min(k + 1, n - 1);

        float p0 = df[layer0].at<float>(y, x);
        float p1 = df[layer1].at<float>(y, x);
        secants[k] = (p1 - p0);
        secants[k] /= float(get(colorTable, layer1) - get(colorTable, layer0));

    }

    for (int k = 1; k < n - 1; k++) {
        tangents[k] = (secants[k - 1] + secants[k]) / 2.0f;
        if (secants[k - 1] * secants[k] < 0) {
            // opposite signs
            tangents[k] = 0.0f;
        }
    }
    tangents[0] = secants[0];
    tangents[n - 1] = secants[n - 2];

    for (int k = 0; k < n - 1; k++) {
        if (secants[k] <= 0.00001f) {
            tangents[k] = 0.0f;

            tangents[k + 1] = 0.0f;
        }
    }
    for (int k = 0; k < n - 1; k++) {
        float alpha = tangents[k] / secants[k];
        float beta = tangents[k + 1] / secants[k];
        if (alpha > 3.0f || beta > 3.0f) {
            tangents[k] = 3 * secants[k];
        }
    }
}

float nitro::CubicInterpolatorySampler::distFuncIndexed(const cv::Mat &colorTable,
                                                        const std::vector<cv::Mat> &df,
                                                        int x, int y, float p, int numLevelsInput,
                                                        std::vector<float> &tangents) const {

    // find the index values that sit in between this value
    //  float oldGray = p * vals[vals.size() - 1];
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

    float delta = get(colorTable, layer1) - get(colorTable, layer0);
    float t = (oldGray - get(colorTable, layer0)) / delta;

    float p0 = df[layer0].at<float>(y, x);
    float p1 = df[layer1].at<float>(y, x);
    float m0 = tangents[layer0];
    float m1 = tangents[layer1];

    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = 2 * t3 - 3 * t2 + 1;
    float b1 = t3 - 2 * t2 + t;
    float b2 = -2 * t3 + 3 * t2;
    float b3 = t3 - t2;

    // Finite dif
    return b0 * p0 + b1 * m0 + b2 * p1 + b3 * m1;
}

cv::Mat nitro::CubicInterpolatorySampler::resample(const cv::Mat &colTable,
                                                   const std::vector<cv::Mat> &df,
                                                   int numDesiredLevels) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled(height, width, CV_8U);

    int numLevelsInput = df.size();
#pragma omp parallel default(none) shared(df, resampled, colTable) firstprivate(width, height, numDesiredLevels, numLevelsInput)
    {
        std::vector<float> tangents(numLevelsInput);
        std::vector<float> secants(numLevelsInput);
#pragma omp for
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Compute interpolation tangents
                computeMonotonicTangents(colTable, df, x, y, tangents, secants);

                for (int d = numDesiredLevels - 1; d >= 0; d--) {
                    float p = d / (numDesiredLevels - 1.0f);
                    float dist = distFuncIndexed(colTable, df, x, y, p, numLevelsInput, tangents);
                    if (dist <= 0.0f) {
                        resampled.at<uchar>(y, x) = d;
                        break;
                    }
                }
            }
        }
    }
    return resampled;
}
