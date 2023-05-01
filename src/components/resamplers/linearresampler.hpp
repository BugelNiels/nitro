#pragma once

#include "resampler.hpp"

namespace nitro {

    class LinearSampler : public Resampler {
    public:
        LinearSampler();

        ~LinearSampler() override;

    protected:
        float distFunc(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                       int numLevelsInput) const override;

        float distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                              int numLevelsInput) const override;
    };

}  // namespace nitro
