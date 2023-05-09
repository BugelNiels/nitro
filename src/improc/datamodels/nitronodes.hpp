#pragma once

// General
#include "invaliddata.hpp"

#include "operators/flipdatamodel.hpp"

// Input
#include "input/imagesourcedatamodel.hpp"
#include "input/integersourcedatamodel.hpp"
#include "input/decimalsourcedatamodel.hpp"

// Output
#include "output/imageviewerdatamodel.hpp"
#include "output/surfaceviewerdatamodel.hpp"

// Filters
#include "operators/filters/lowpassdatamodel.hpp"

// Conversions
#include "conversions/tograyscaledatamodel.hpp"
#include "conversions/rgb/separatergbdatamodel.hpp"
#include "src/improc/datamodels/operators/filters/thresholddatamodel.hpp"
#include "conversions/rgb/combinergbdatamodel.hpp"
#include "conversions/ycbcr/combineycbcrdatamodel.hpp"
#include "conversions/ycbcr/separateycbcrdatamodel.hpp"

// Quantization
#include "operators/quantization/kmeansdatamodel.hpp"
#include "operators/quantization/quantisizedatamodel.hpp"
#include "operators/quantization/slrdatamodel.hpp"

// Reconstruction
#include "operators/reconstruction/resampledatamodel.hpp"

// Util
#include "operators/util/toggledatamodel.hpp"
#include "operators/util/blenddatamodel.hpp"
#include "operators/util/imgmathdatamodel.hpp"
#include "operators/util/mixdatamodel.hpp"