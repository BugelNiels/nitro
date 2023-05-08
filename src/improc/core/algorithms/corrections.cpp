#include "corrections.hpp"

#include "src/improc/core/algorithms/util/imginfo.hpp"
#include "src/improc/core/algorithms/util/imgmath.hpp"
#include "src/improc/core/algorithms/conversions/ycbcrconvert.hpp"

QImage nitro::operations::brightnessCorrect(const QImage &image, const nitro::CbdImage &image1) {
    auto channels = nitro::operations::separateYCbCr(image);

    float meanImg = nitro::operations::meanBrightness(channels[0]);
    float meanRef = nitro::operations::meanBrightness(image1);

    float diff = meanImg - meanRef;
    channels[0] = subtractImage(CbdImage(channels[0]), diff).getDisplayImg();
    return nitro::operations::combineYCbCr(channels[0], channels[1], channels[2]);
}

nitro::CbdImage nitro::operations::brightnessCorrect(const nitro::CbdImage &image, const nitro::CbdImage &image1) {
    float meanImg = nitro::operations::meanBrightness(image);
    float meanRef = nitro::operations::meanBrightness(image1);
    float diff = meanImg - meanRef;
    return subtractImage(image, diff);
}