#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <QImage>

class Resampler {
 public:
  Resampler();
  virtual ~Resampler();
  virtual QImage resample(QImage& image, int dynRangeInput,
                          const QVector<float**>& distanceField,
                          int numLevels) = 0;
};

#endif  // RESAMPLER_H
