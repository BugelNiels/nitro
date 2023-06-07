#pragma once

#include "src/core/cbdimage.hpp"

namespace nitro::operations {

    int numComponents(const nitro::CbdImage& image, int greyVal);

    std::vector<int> componentHistogram(const nitro::CbdImage& image);

} // nitro
