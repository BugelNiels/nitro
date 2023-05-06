#include "threshold.hpp"

nitro::CbdImage nitro::operations::threshold(const CbdImage &inputImg, int threshold, bool greater) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, 2);

    auto &inData = inputImg.constData();
    auto &outData = result.data();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int val = inData.get(x, y);
            bool satisfies = greater ? val >= threshold : val <= threshold;
            outData.set(x, y, satisfies ? 1 : 0);
        }
    }
    result.setIndexed({0, 255});
    return result;
}
