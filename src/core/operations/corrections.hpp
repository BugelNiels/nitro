#pragma once

#include "cbdimage.hpp"
#include <QImage>

namespace nitro::operations {
    nitro::CbdImage brightnessCorrect(const nitro::CbdImage &image, const nitro::CbdImage &image1);
    QImage brightnessCorrect(const QImage &image, const nitro::CbdImage &image1);
} // nitro
