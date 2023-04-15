#include "cubichermiteresampler.h"

#include <QDebug>

CubicHermiteSampler::CubicHermiteSampler() {}

CubicHermiteSampler::~CubicHermiteSampler() {}

static inline float distFunc(const QVector<Matrix<float>> &sdf, int x, int y,
                             int greyLevel, int numLevelsInput,
                             int numDesiredLevels) {
  float p = greyLevel / (numDesiredLevels - 1.0f);

  int layer0 = p * numLevelsInput;
  int layer_1 = std::max(layer0 - 1, 0);
  int layer1 = std::min(layer0 + 1, numLevelsInput);
  int layer2 = std::min(layer1 + 1, numLevelsInput);

  float t = p * numLevelsInput - layer0;

  float p_1 = sdf[layer_1].get(x, y);
  float p0 = sdf[layer0].get(x, y);
  float p2 = sdf[layer2].get(x, y);
  float p1 = sdf[layer1].get(x, y);

  float t2 = t * t;
  float t3 = t2 * t;

  float b0 = 2 * t3 - 3 * t2 + 1;
  float b1 = t3 - 2 * t2 + t;
  float b2 = -2 * t3 + 3 * t2;
  float b3 = t3 - t2;

  // Catmull Rom
  float m0 = (p1 - p_1) / ((float)(layer1 - layer_1)) * 0.5f;
  float m1 = (p2 - p0) / ((float)(layer2 - layer0)) * 0.5;

  // temp fix to prevent cubic spline breaking monotonicity
  if (layer1 + 1 >= numLevelsInput) {
    m1 = 1.0;
  }

  // Finite dif
  return b0 * p0 + b1 * m0 + b2 * p1 + b3 * m1;
}

QImage CubicHermiteSampler::resample(QImage &image,
                                     const QVector<Matrix<float>> &sdf,
                                     int numDesiredLevels) {
  int width = image.width();
  int height = image.height();

  QImage resampled(width, height, QImage::Format_Grayscale8);
  resampled.fill(0);

  int numLevelsInput = sdf.size();
  for (int y = 0; y < height; y++) {
    uchar *resampledRow = resampled.scanLine(y);
    for (int x = 0; x < width; x++) {
      for (int d = numDesiredLevels - 1; d >= 0; d--) {
        float dist =
            distFunc(sdf, x, y, d, numLevelsInput - 1, numDesiredLevels);
        if (dist < 0) {
          resampledRow[x] = d;
          break;
        }
      }
    }
  }

  return resampled;
}
