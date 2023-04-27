#pragma once

#include "resampler.hpp"

namespace nitro {

    class LinearSampler : public Resampler {
    public:
        LinearSampler();

        ~LinearSampler() override;

    protected:
        float distFunc(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;

        float distFuncIndexed(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;
    };

}  // namespace nitro
