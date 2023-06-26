#pragma once

#include <QImage>
#include <opencv2/core/mat.hpp>
#include "sampler.hpp"

namespace nitro {

    class SmoothSampler  : public Sampler {
    public:
        SmoothSampler();

    protected:
        [[nodiscard]] float mix(float a, float b, float w) const override;
    };
}  // namespace nitro
