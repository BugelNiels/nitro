#ifndef SAMPLEMETHOD_H
#define SAMPLEMETHOD_H

#include <QMap>

#include "resamplers/cubichermiteresampler.h"
#include "resamplers/cubicinterpolatoryresampler.h"
#include "resamplers/linearresampler.h"
#include "resamplers/resampler.h"

enum SampleMethod { LINEAR, CUBIC_HERMITE, FRITSCH_CARLSON };

static const QMap<SampleMethod, std::shared_ptr<Resampler>> samplers{
    {SampleMethod::LINEAR, std::shared_ptr<Resampler>(new LinearSampler())},
    {SampleMethod::CUBIC_HERMITE,
     std::shared_ptr<Resampler>(new CubicHermiteSampler())},
    {SampleMethod::FRITSCH_CARLSON,
     std::shared_ptr<Resampler>(new CubicInterpolatorySampler())},
};

std::shared_ptr<Resampler> getSampler(SampleMethod method);

#endif  // SAMPLEMETHOD_H
