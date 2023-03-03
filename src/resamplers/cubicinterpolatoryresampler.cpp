#include "cubicinterpolatoryresampler.h"

#include <QDebug>

CubicInterpolatorySampler::CubicInterpolatorySampler() {}

CubicInterpolatorySampler::~CubicInterpolatorySampler() {}

float CubicInterpolatorySampler::distFunc(const QVector<float **> &sdf, int x,
                                          int y, int greyLevel,
                                          int numLevelsInput,
                                          int numDesiredLevels) {
  float p = greyLevel / (numDesiredLevels - 1.0f);

  int layer0 = p * numLevelsInput;
  int layer1 = std::min(layer0 + 1, numLevelsInput);

  float t = p * numLevelsInput - layer0;

  float p0 = sdf[layer0][y][x];
  float p1 = sdf[layer1][y][x];
  float m0 = tangents[layer0];
  float m1 = tangents[layer1];

  float t2 = t * t;
  float t3 = t2 * t;

  float b0 = 2 * t3 - 3 * t2 + 1;
  float b1 = t3 - 2 * t2 + t;
  float b2 = -2 * t3 + 3 * t2;
  float b3 = t3 - t2;

  // Finite dif
  return b0 * p0 + b1 * m0 + b2 * p1 + b3 * m1;
}

/**
 * @brief CubicInterpolatorySampler::computeMonotonicTangents Computes the
 * monotinic tangents required for the Fitsch-Carlson method here.
 * @param distanceField Collection of distance fields from which to calculate
 * the tangents. Only calculates it for 1 single coordinate.
 * @param x The x coordinate in the distance field.
 * @param y The y coordinate in the distance field.
 * @param n The number of data points to calculate tangents for.
 */
void CubicInterpolatorySampler::computeMonotonicTangents(
    const QVector<float **> &distanceField, int x, int y, int n) {
  // This performs the first step of averaging the tangents
  for (int k = 0; k < n; k++) {
    // index k for tangent computation
    int layer0 = k;
    // index k + 1 for tangent computation
    int layer1 = std::min(k + 1, n - 1);

    // tangent delta_k
    float p0 = distanceField[layer0][y][x];
    // tangent delta_k+1
    float p1 = distanceField[layer1][y][x];
    tangents[k] = (p1 - p0) / (float)(layer1 - layer0);
  }
  // This performs the second step of ensuring monotonicity
  for (int k = 0; k < n - 1; k++) {
    // Calculate minimum and maximum slope between the two neighboring tangents
    float minSlope = std::min(tangents[k], tangents[k + 1]);
    float maxSlope = std::max(tangents[k], tangents[k + 1]);

    // Ensure monotonicity by forcing the tangent to be within the bounds of the
    // min and max slope
    //    tangents[k] = std::max(tangents[k], minSlope);
    //    tangents[k] = std::min(tangents[k], maxSlope);
    //    tangents[k + 1] = tangents[k];
    tangents[k + 1] = std::max(std::min(tangents[k + 1], maxSlope), minSlope);
  }
  tangents[0] = tangents[1];
  tangents[n - 1] = tangents[n - 2];
}

QImage CubicInterpolatorySampler::resample(QImage &image,
                                           const QVector<float **> &sdf,
                                           int numDesiredLevels) {
  int width = image.width();
  int height = image.height();

  QImage resampled(width, height, QImage::Format_Grayscale8);
  resampled.fill(0);

  int numLevelsInput = sdf.size();
  tangents.resize(numLevelsInput);
  for (int y = 0; y < height; y++) {
    uchar *resampledRow = resampled.scanLine(y);
    for (int x = 0; x < width; x++) {
      // Compute interpolation tangents

      computeMonotonicTangents(sdf, x, y, numLevelsInput);

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
