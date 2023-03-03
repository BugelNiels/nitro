#include "linearresampler.h"

#include <QDebug>

LinearResampler::LinearResampler() {}

LinearResampler::~LinearResampler() {}

float mix(float a, float b, float w) { return (1.0f - w) * a + w * b + 0.5f; }

static inline float distFunc(const QVector<float **> &distanceField, int x,
                             int y, int greyLevel, int dynRange,
                             int numGreyLevels) {
  float p = greyLevel / (numGreyLevels - 1.0f);

  int layer1 = p * dynRange;
  int layer2 = std::min(layer1 + 1, dynRange);
  float w = p * dynRange - layer1;

  float d1 = distanceField[layer1][y][x];
  float d2 = distanceField[layer2][y][x];
  return mix(d1, d2, w);
}

QImage LinearResampler::resample(QImage &image, int dynRangeInput,
                                 const QVector<float **> &distanceField,
                                 int numLevels) {
  int width = image.width();
  int height = image.height();

  QImage resampled(width, height, QImage::Format_Grayscale8);
  resampled.fill(0);

  for (int y = 0; y < height; y++) {
    uchar *resampledRow = resampled.scanLine(y);
    for (int x = 0; x < width; x++) {
      for (int d = numLevels - 1; d >= 0; d--) {
        float dist = distFunc(distanceField, x, y, d, dynRangeInput, numLevels);
        if (dist < 0) {
          resampledRow[x] = d;
          break;
        }
      }
    }
  }

  return resampled;
}
