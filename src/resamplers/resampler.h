#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <QImage>

class Resampler {
 public:
  Resampler();
  virtual ~Resampler();
  virtual QImage resample(QImage& image, const QVector<float**>& distanceField,
                          int numDesiredLevels) = 0;
};

#endif  // RESAMPLER_H
