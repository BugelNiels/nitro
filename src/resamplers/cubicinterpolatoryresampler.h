#ifndef CUBIC_INTERPOLATORY_SAMPLER_H
#define CUBIC_INTERPOLATORY_SAMPLER_H

#include "resampler.h"

class CubicInterpolatorySampler : public Resampler {
 public:
  CubicInterpolatorySampler();
  ~CubicInterpolatorySampler() override;

  QImage resample(QImage& image, const QVector<Matrix<float>>& sdf,
                  int numDesiredLevels) override;

 private:
  void computeMonotonicTangents(const QVector<Matrix<float>>& distanceField,
                                int x, int y, int n);
  float distFunc(const QVector<Matrix<float>>& sdf, int x, int y, int greyLevel,
                 int numLevelsInput, int numDesiredLevels);
  QVector<float> tangents;
};

#endif  // CUBIC_INTERPOLATORY_SAMPLER_H
