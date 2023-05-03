#include "cubicinterpolatoryresampler.hpp"

#include <QDebug>

nitro::CubicInterpolatorySampler::CubicInterpolatorySampler() {}

nitro::CubicInterpolatorySampler::~CubicInterpolatorySampler() {}

/**
 * @brief CubicInterpolatorySampler::computeMonotonicTangents Computes the
 * monotinic tangents required for the Fitsch-Carlson method here.
 * @param distanceField Collection of distance fields from which to calculate
 * the tangents. Only calculates it for 1 single coordinate.
 * @param x The x coordinate in the distance field.
 * @param y The y coordinate in the distance field.
 * @param n The number of data points to calculate tangents for.
 */
void nitro::CubicInterpolatorySampler::computeMonotonicTangents(const CbdImage &image, const DistanceField &df, int x,
                                                                int y, QVector<float> &tangents,
                                                                QVector<float> &secants) {
    int n = image.numLevels();
    const auto &vals = image.getColTransform();
    // This performs the first step of averaging the tangents
    for (int k = 0; k < n - 1; k++) {
        // index k for tangent computation
        int layer0 = k;
        // index k + 1 for tangent computation
        int layer1 = k + 1;  // std::min(k + 1, n - 1);

        float p0 = df.getDistField(layer0).get(x, y);
        float p1 = df.getDistField(layer1).get(x, y);
        secants[k] = (p1 - p0);
        if (image.isIndexed()) {
            secants[k] /= float(vals[layer1] - vals[layer0]);
        }
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

float nitro::CubicInterpolatorySampler::distFunc(const CbdImage &image, const DistanceField &df, int x, int y,
                                                 float p, int numLevelsInput, const QVector<float> &tangents) const {
    numLevelsInput -= 1;
    int layer0 = p * numLevelsInput;
    int layer1 = MIN(layer0 + 1, numLevelsInput);

    float t = p * numLevelsInput - layer0;

    float p0 = df.getDistField(layer0).get(x, y);
    float p1 = df.getDistField(layer1).get(x, y);
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

float nitro::CubicInterpolatorySampler::distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y,
                                                        float p,
                                                        int numLevelsInput, const QVector<float> &tangents) const {
    const auto &vals = image.getColTransform();

    // find the index values that sit in between this value
    //  float oldGray = p * vals[vals.size() - 1];
    float oldGray = p * 255.0f;

    int layer0 = -1;

    for (int i = 0; i < numLevelsInput - 1; i++) {
        if (oldGray >= vals[i] && oldGray <= vals[i + 1]) {
            layer0 = i;
            break;
        }
    }
    if (layer0 == -1) {
        if (oldGray >= vals[numLevelsInput - 1]) {
            layer0 = numLevelsInput - 1;
        } else {
            layer0 = 0;
        }
    }

    int layer1 = MIN(layer0 + 1, numLevelsInput - 1);

    float delta = vals[layer1] - vals[layer0];
    float t = (oldGray - vals[layer0]) / delta;

    float p0 = df.getDistField(layer0).get(x, y);
    float p1 = df.getDistField(layer1).get(x, y);
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

nitro::CbdImage nitro::CubicInterpolatorySampler::resample(const CbdImage &image, const DistanceField &df,
                                                           int numDesiredLevels) {
    int width = image.width();
    int height = image.height();

    CbdImage resampled(width, height, numDesiredLevels);
    auto &resampledData = resampled.data();

    int numLevelsInput = image.numLevels();
#pragma omp parallel default(none) shared(image, df, resampledData) firstprivate(width, height, numDesiredLevels, numLevelsInput)
    {
        QVector<float> tangents(numLevelsInput);
        QVector<float> secants(numLevelsInput);
#pragma omp for
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Compute interpolation tangents
                computeMonotonicTangents(image, df, x, y, tangents, secants);

                for (int d = numDesiredLevels - 1; d >= 0; d--) {
                    float p = d / (numDesiredLevels - 1.0f);
                    float dist;
                    if (image.isIndexed()) {
                        dist = distFuncIndexed(image, df, x, y, p, numLevelsInput, tangents);
                    } else {
                        dist = distFunc(image, df, x, y, p, numLevelsInput, tangents);
                    }
                    if (dist <= 0.0f) {
                        resampledData.set(x, y, d);
                        break;
                    }
                }
            }
        }
    }
    if (numDesiredLevels != 256) {
        QVector<int> vals(numDesiredLevels);
        for (int i = 0; i < numDesiredLevels; i++) {
            vals[i] = i * 255.0 / float(numDesiredLevels - 1);
        }
        resampled.setIndexed(vals);
    }

    return resampled;
}
