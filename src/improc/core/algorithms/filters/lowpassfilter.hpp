#pragma once

#include "modules/imCore/internal/cbdimage.hpp"

namespace nitro::operations {

    CbdImage lowPassFilter(const CbdImage& image, float cutoff, int order);

} // nitro
