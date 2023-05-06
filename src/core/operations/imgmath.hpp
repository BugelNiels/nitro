#pragma once

#include "cbdimage.hpp"
#include <QImage>

namespace nitro::operations {

    nitro::CbdImage addImage(const nitro::CbdImage &inputImg, int val);

    nitro::CbdImage subtractImage(const nitro::CbdImage &inputImg, int val);

    nitro::CbdImage multiplyImage(const nitro::CbdImage &inputImg, double val);

    nitro::CbdImage divideImage(const nitro::CbdImage &inputImg, double val);

    QImage mixImage(const QImage& imgA, const QImage& imgB, float factor);

} // nitro::operations