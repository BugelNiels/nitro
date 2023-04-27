#include "cubichermiteresampler.hpp"

#include <QDebug>

nitro::CubicHermiteSampler::CubicHermiteSampler() = default;

nitro::CubicHermiteSampler::~CubicHermiteSampler() = default;

float nitro::CubicHermiteSampler::distFunc(CbdImage &image, int x, int y, float p,
                                           int numLevelsInput) const {
    const auto &sdf = image.getDistField();
    numLevelsInput -= 1;
    int layer0 = p * numLevelsInput;
    int layer_1 = MAX(layer0 - 1, 0);
    int layer1 = MIN(layer0 + 1, numLevelsInput);
    int layer2 = MIN(layer1 + 1, numLevelsInput);

    float t = p * numLevelsInput - layer0;

    float p_1 = sdf[layer_1].get(x, y);
    float p0 = sdf[layer0].get(x, y);
    float p2 = sdf[layer2].get(x, y);
    float p1 = sdf[layer1].get(x, y);

    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = 2 * t3 - 3 * t2 + 1;
    float b1 = t3 - 2 * t2 + t;
    float b2 = -2 * t3 + 3 * t2;
    float b3 = t3 - t2;

    // Catmull Rom
    float m0 = (p1 - p_1) / ((float) (layer1 - layer_1)) * 0.5f;
    float m1 = (p2 - p0) / ((float) (layer2 - layer0)) * 0.5;

    // temp fix to prevent cubic spline breaking monotonicity
    if (layer1 + 1 >= numLevelsInput) {
        m1 = 1.0;
    }

    // Finite dif
    return b0 * p0 + b1 * m0 + b2 * p1 + b3 * m1;
}

float nitro::CubicHermiteSampler::distFuncIndexed(CbdImage &image, int x, int y,
                                                  float p, int numLevelsInput) const {
    const auto &vals = image.getColTransform();
    const auto &sdf = image.getDistField();

    // find the index values that sit in between this value
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

    int layer_1 = MAX(layer0 - 1, 0);
    int layer1 = MIN(layer0 + 1, numLevelsInput - 1);
    int layer2 = MIN(layer1 + 1, numLevelsInput - 1);

    float t = (oldGray - vals[layer0]) / float(vals[layer1] - vals[layer0]);

    float p_1 = sdf[layer_1].get(x, y);
    float p0 = sdf[layer0].get(x, y);
    float p2 = sdf[layer2].get(x, y);
    float p1 = sdf[layer1].get(x, y);

    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = 2 * t3 - 3 * t2 + 1;
    float b1 = t3 - 2 * t2 + t;
    float b2 = -2 * t3 + 3 * t2;
    float b3 = t3 - t2;

    // Catmull Rom
    float m0 = (p1 - p_1) / ((float) (layer1 - layer_1)) * 0.5f;
    float m1 = (p2 - p0) / ((float) (layer2 - layer0)) * 0.5;

    // temp fix to prevent cubic spline breaking monotonicity
    if (layer1 + 1 >= numLevelsInput) {
        m1 = 1.0;
    }

    // Finite dif
    return b0 * p0 + b1 * m0 + b2 * p1 + b3 * m1;
}
