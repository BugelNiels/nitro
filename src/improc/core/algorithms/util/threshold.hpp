#pragma once

#include "src/improc/core/cbdimage.hpp"

namespace nitro::operations {

    CbdImage threshold(const CbdImage &inputImg, int threshold, bool greater = false);

} // nitro::operations
