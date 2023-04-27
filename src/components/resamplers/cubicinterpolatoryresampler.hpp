#pragma once

#include "resampler.hpp"

namespace nitro {

    class CubicInterpolatorySampler : public Resampler {
    public:
        CubicInterpolatorySampler();

        ~CubicInterpolatorySampler() override;

        CbdImage resample(CbdImage &image, int numDesiredLevels) override;

    protected:
        float distFunc(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;

        float distFuncIndexed(CbdImage &image, int x, int y, float p, int numLevelsInput) const override;

    private:
        void computeMonotonicTangents(CbdImage &image, int x, int y);

        QVector<float> tangents;
        QVector<float> secants;
    };

}  // namespace nitro
