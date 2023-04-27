#pragma once

#include <QMap>

#include "resamplers/cubichermiteresampler.hpp"
#include "resamplers/cubicinterpolatoryresampler.hpp"
#include "resamplers/linearresampler.hpp"
#include "resamplers/resampler.hpp"

namespace nitro {

    enum SampleMethod {
        LINEAR, CUBIC_HERMITE, FRITSCH_CARLSON
    };

    static const QMap<SampleMethod, std::shared_ptr<Resampler>> samplers{
            {SampleMethod::LINEAR,          std::shared_ptr<Resampler>(new LinearSampler())},
            {SampleMethod::CUBIC_HERMITE,   std::shared_ptr<Resampler>(new CubicHermiteSampler())},
            {SampleMethod::FRITSCH_CARLSON, std::shared_ptr<Resampler>(new CubicInterpolatorySampler())},
    };

    std::shared_ptr<Resampler> getSampler(SampleMethod method);

}  // namespace nitro
