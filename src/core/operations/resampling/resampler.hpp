#pragma once

#include <QImage>

#include "cbdimage.hpp"
#include "src/core/operations/distancefield.hpp"

namespace nitro {

    class Resampler {
    public:
        Resampler();

        virtual ~Resampler();

        virtual CbdImage resample(const CbdImage &image, const DistanceField &df, int numDesiredLevels);

    protected:
        virtual float
        distFunc(const CbdImage &image, const DistanceField &df, int x, int y, float p, int numLevelsInput) const = 0;

        virtual float distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                                      int numLevelsInput) const = 0;
    };
}  // namespace nitro
