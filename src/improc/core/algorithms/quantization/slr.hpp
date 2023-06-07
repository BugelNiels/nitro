#pragma once

#include "src/core/cbdimage.hpp"

namespace nitro::operations {
    nitro::CbdImage histLayerRemoval(const nitro::CbdImage &img, int k);
    nitro::CbdImage componentLayerRemoval(const nitro::CbdImage &img, int k);
} // nitro
