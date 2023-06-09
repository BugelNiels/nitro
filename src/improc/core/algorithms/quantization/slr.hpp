#pragma once

#include "modules/imCore/internal/cbdimage.hpp"

namespace nitro::operations {
    nitro::CbdImage histLayerRemoval(const nitro::CbdImage &img, int k);
    nitro::CbdImage componentLayerRemoval(const nitro::CbdImage &img, int k);
} // nitro
