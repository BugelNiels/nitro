#include "linearresampler.h"

#include <QDebug>

LinearSampler::LinearSampler() {}

LinearSampler::~LinearSampler() {}

static float mix(float a, float b, float w) {
  return (1.0f - w) * a + w * b + 0.5f;
}

static inline float distFunc(const QVector<float **> &distanceField, int x,
                             int y, int greyLevel, int numLevelsInput,
                             int numDesiredLevels) {
  float p = greyLevel / (numDesiredLevels - 1.0f);

  int layer1 = p * numLevelsInput;
  int layer2 = std::min(layer1 + 1, numLevelsInput);

  float t = p * numLevelsInput - layer1;

  float d1 = distanceField[layer1][y][x];
  float d2 = distanceField[layer2][y][x];
  return mix(d1, d2, t);
}

QImage LinearSampler::resample(QImage &image, const QVector<float **> &sdf,
                               int numDesiredLevels) {
  int width = image.width();
  int height = image.height();

  QImage resampled(width, height, image.format());
  resampled.fill(0);

  int numLevelsInput = sdf.size();
  qDebug() << numLevelsInput;
  for (int y = 0; y < height; y++) {
    uchar *resampledRow = resampled.scanLine(y);
    uchar *inputRow = image.scanLine(y);
    for (int x = 0; x < width; x++) {
      //      for (int d = numLevelsInput - 1; d >= 0; d--) {
      //        float dist = sdf[d][y][x];
      //        if (dist <= 0.0f) {
      //          float p = d / (numLevelsInput - 1.0f);
      //          resampledRow[x] = qRound(p * 255.0f);
      //          break;
      //        }
      //      }
      for (int d = numDesiredLevels - 1; d >= 0; d--) {
        float dist =
            distFunc(sdf, x, y, d - 1, numLevelsInput - 1, numDesiredLevels);
        if (dist <= 1) {
          float p = d / (numDesiredLevels - 1.0f);
          resampledRow[x] = p * 255.0f;
          //                resampledRow[x] = d;
          break;
        }
      }
    }
  }

  return resampled;
}
