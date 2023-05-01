#pragma once

#include "resampler.hpp"

namespace nitro {

    class CubicInterpolatorySampler : public Resampler {
    public:
        CubicInterpolatorySampler();

        ~CubicInterpolatorySampler() override;

        CbdImage resample(const CbdImage &image, const DistanceField &df, int numDesiredLevels) override;

    protected:
        float distFunc(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                       int numLevelsInput) const override;

        float distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                              int numLevelsInput) const override;

    private:
        void computeMonotonicTangents(const CbdImage &image, const DistanceField &df, int x, int y);

        QVector<float> tangents;
        QVector<float> secants;
    };

}  // namespace nitro
