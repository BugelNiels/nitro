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
                       int numLevelsInput, const QVector<float> &tangents) const;

        float distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                              int numLevelsInput, const QVector<float> &tangents) const;

        // Kind of hacky
        float distFunc(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                       int numLevelsInput) const override { return 0; }

        float distFuncIndexed(const CbdImage &image, const DistanceField &df, int x, int y, float p,
                              int numLevelsInput) const override { return 0; }

    private:
        void
        computeMonotonicTangents(const CbdImage &image, const DistanceField &df, int x, int y, QVector<float> &tangents,
                                 QVector<float> &secants);

    };

}  // namespace nitro
