#pragma once

#include "cbdimage.hpp"

namespace nitro::operations {
    nitro::CbdImage uniformQuantizationDither(const nitro::CbdImage &img, int k);
    nitro::CbdImage uniformQuantization(const nitro::CbdImage &img, int k);

} // nitro::operations
