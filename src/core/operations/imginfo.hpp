#pragma once

#include "cbdimage.hpp"
#include <QImage>

namespace nitro::operations {

    float meanBrightness(const nitro::CbdImage& image);
    float meanBrightness(const QImage& image);

} // nitro
