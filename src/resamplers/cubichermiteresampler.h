#ifndef CUBIC_HERMITE_SAMPLER_H
#define CUBIC_HERMITE_SAMPLER_H

#include "resampler.h"

class CubicHermiteSampler : public Resampler {
 public:
  CubicHermiteSampler();
  ~CubicHermiteSampler() override;

  QImage resample(QImage& image, const QVector<Matrix<float>>& sdf,
                  int numDesiredLevels) override;
};

#endif  // CUBIC_HERMITE_SAMPLER_H
