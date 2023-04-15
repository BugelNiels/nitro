#ifndef LINEARRESAMPLER_H
#define LINEARRESAMPLER_H

#include "resampler.h"

class LinearSampler : public Resampler {
 public:
  LinearSampler();
  ~LinearSampler() override;

  QImage resample(QImage& image, const QVector<Matrix<float>>& sdf,
                  int numDesiredLevels) override;
};

#endif  // LINEARRESAMPLER_H
