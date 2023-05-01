#include "linearresampler.hpp"

#include <QDebug>

nitro::LinearSampler::LinearSampler() = default;

nitro::LinearSampler::~LinearSampler() = default;

static inline float mix(float a, float b, float w) {
    return (1.0f - w) * a + w * b + 0.5f;
}

float nitro::LinearSampler::distFunc(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                                     int numLevelsInput) const {
    numLevelsInput -= 1;
    int layer0 = p * numLevelsInput;
    int layer1 = MIN(layer0 + 1, numLevelsInput);

    float t = p * numLevelsInput - layer0;

    float d1 = df.getDistField(layer0).get(x, y);
    float d2 = df.getDistField(layer1).get(x, y);
    return mix(d1, d2, t);
}

float nitro::LinearSampler::distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                                            int numLevelsInput) const {
    const auto &vals = image.getColTransform();

    // find the index values that sit in between this value
    float oldGray = p * 255.0f;

    //  int layer1 = 0;

    //  for (int i = 0; i < vals.size() - 1; i++) {
    //    if (vals[i] != -1 && oldGray >= vals[i] && oldGray <= vals[i + 1]) {
    //      layer1 = i;
    //      break;
    //    }
    //  }
    //  if (layer1 == 0 && vals[0] == -1) {
    //    while (vals[layer1] == -1) layer1++;
    //  }
    //  int last = vals.size() - 1;
    //  while (vals[last] == -1) last--;
    //  if (layer1 == 0 && oldGray > vals[last]) {
    //    layer1 = last;
    //  }

    //  //  int layer1 = p * numLevelsInput;
    //  int layer2 = std::min(layer1 + 1, numLevelsInput);
    //  while (layer2 != numLevelsInput - 1 && vals[layer2] == -1) {
    //    layer2++;
    //  }
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

    float t = (oldGray - vals[layer0]) / float(vals[layer1] - vals[layer0]);

    float d1 = df.getDistField(layer0).get(x, y);
    float d2 = df.getDistField(layer1).get(x, y);
    return mix(d1, d2, t);
}
