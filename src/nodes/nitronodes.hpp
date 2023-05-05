#pragma once


#include "invaliddata.hpp"

#include "input/imagesourcedatamodel.hpp"
#include "conversions/tograyscaledatamodel.hpp"
#include "conversions/rgb/separatergbdatamodel.hpp"
#include "operators/thresholddatamodel.hpp"
#include "output/imageviewerdatamodel.hpp"
#include "output/surfaceviewerdatamodel.hpp"
#include "operators/quantization/kmeansdatamodel.hpp"
#include "operators/quantization/quantisizedatamodel.hpp"
#include "operators/flipdatamodel.hpp"
#include "operators/math/imgmathdatamodel.hpp"
#include "operators/reconstruction/resampledatamodel.hpp"
#include "conversions/rgb/combinergbdatamodel.hpp"
#include "conversions/ycbcr/combineycbcrdatamodel.hpp"
#include "conversions/ycbcr/separateycbcrdatamodel.hpp"
#include "operators/reconstruction/lumcorrectdatamodel.hpp"
#include "operators/toggledatamodel.hpp"