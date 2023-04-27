#pragma once

#include <QImage>

#include "cbdimage.hpp"

namespace nitro {

    class Resampler {
    public:
        Resampler();

        virtual ~Resampler();

        virtual CbdImage resample(CbdImage &image, int numDesiredLevels);

    protected:
        virtual float distFunc(CbdImage &image, int x, int y, float p, int numLevelsInput) const = 0;

        virtual float distFuncIndexed(CbdImage &image, int x, int y, float p, int numLevelsInput) const = 0;
    };
}  // namespace nitro
