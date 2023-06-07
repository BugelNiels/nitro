#pragma once

#include "src/core/cbdimage.hpp"

namespace nitro::operations {

    CbdImage lowPassFilter(const CbdImage& image, float cutoff, int order);

} // nitro
