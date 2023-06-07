#pragma once

#include "src/core/cbdimage.hpp"

namespace nitro::operations {
    nitro::CbdImage kMeansDither(const nitro::CbdImage &img, int k, int iter);
    nitro::CbdImage kMeans(const nitro::CbdImage &img, int k, int iter);
} // nitro