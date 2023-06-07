#pragma once

#include "src/core/cbdimage.hpp"
#include <QImage>

namespace nitro::operations {
    nitro::CbdImage brightnessCorrect(const nitro::CbdImage &image, const nitro::CbdImage &image1, bool local);
    QImage brightnessCorrect(const QImage &image, const nitro::CbdImage &image1, bool local);
} // nitro
