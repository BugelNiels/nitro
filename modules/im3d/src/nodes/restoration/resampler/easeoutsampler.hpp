#pragma once

#include <QImage>
#include <opencv2/core/mat.hpp>
#include "sampler.hpp"

namespace nitro {

    class EaseOutSampler  : public Sampler {
    public:
        EaseOutSampler();

    protected:
        [[nodiscard]] float mix(float a, float b, float w) const override;
    };
}  // namespace nitro
