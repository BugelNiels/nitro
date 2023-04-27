#pragma once

#include "resampler.hpp"

namespace nitro {

    class CubicHermiteSampler : public Resampler {
    public:
        CubicHermiteSampler();

        ~CubicHermiteSampler() override;

    protected:
        float distFunc(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;

        float distFuncIndexed(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;
    };

}  // namespace nitro
