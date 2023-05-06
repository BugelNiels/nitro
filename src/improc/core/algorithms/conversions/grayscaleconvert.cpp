
#include "grayscaleconvert.hpp"

QImage nitro::operations::toGrayScale(const QImage &in) {
    if (in.isGrayscale()) {
        return in;
    }
    return in.convertToFormat(QImage::Format_Grayscale8);
}