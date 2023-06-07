#pragma once

#include "src/core/cbdimage.hpp"
#include <QImage>
#include <QPainter>

namespace nitro::operations {

    nitro::CbdImage addImage(const nitro::CbdImage &inputImg, int val);

    // TODO: overload operators;
    nitro::CbdImage subtractImage(const nitro::CbdImage &inputImg, int val);
    nitro::CbdImage subtractImage(const nitro::CbdImage &inputImg, const nitro::CbdImage &other);

    nitro::CbdImage multiplyImage(const nitro::CbdImage &inputImg, double val);

    nitro::CbdImage divideImage(const nitro::CbdImage &inputImg, double val);

    QImage mixImage(const QImage& imgA, const QImage& imgB, float factor);

    QImage mixImage(const QImage &imgA, const QImage &imgB, QPainter::CompositionMode mode);
} // nitro::operations