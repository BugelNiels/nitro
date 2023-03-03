#ifndef LINEARRESAMPLER_H
#define LINEARRESAMPLER_H

#include "resampler.h"

class LinearResampler : public Resampler {
 public:
  LinearResampler();
  ~LinearResampler() override;

  QImage resample(QImage& image, int dynRangeInput,
                  const QVector<float**>& imValues, int numLevels) override;
};

#endif  // LINEARRESAMPLER_H
