#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <QImage>

#include "matrix.h"

class Resampler {
 public:
  Resampler();
  virtual ~Resampler();
  virtual QImage resample(QImage& image,
                          const QVector<Matrix<float>>& distanceField,
                          int numDesiredLevels) = 0;
};

#endif  // RESAMPLER_H
