#include "imageview.h"

void ImageView::quantisize() {
  resetProgress();
  quantisizedImg = quantisize(originalImg, sampleSettings.quantisizeLevel);
  quantisized = true;

  // We first need to multiply this to get it to show properly
  int width = quantisizedImg.width();
  int height = quantisizedImg.height();
  float scaleFactor = 255.0f / sampleSettings.quantisizeLevel;

  QImage displayQuantImg = QImage(width, height, QImage::Format_Grayscale8);

  for (int y = 0; y < height; y++) {
    const uchar *inputRow = quantisizedImg.constScanLine(y);
    uchar *upscaledRow = displayQuantImg.scanLine(y);
    for (int x = 0; x < width; x++) {
      upscaledRow[x] = inputRow[x] * scaleFactor + 0.5f;
    }
  }
  savedImages.append(displayQuantImg);
  finalizeProgress();
}

static float **allocFloatMatrix(int width, int height) {
  float **matrix = (float **)(malloc(height * sizeof(float *) +
                                     width * height * sizeof(float)));
  float *p = (float *)(matrix + height);
  for (int y = 0; y < height; y++) {
    matrix[y] = p + width * y;
  }
  return matrix;
}

static float computeEuclideanDistance3D(int xp, int yp, QImage &image, int col,
                                        int d, double pixelMult) {
  int width = image.width();
  int height = image.height();
  double minDistance = width * width + height * height;
  for (int y = 0; y < height; y++) {
    int yDiff2 = (yp - y) * (yp - y);
    uchar *inputRow = image.scanLine(y);
    for (int x = 0; x < width; x++) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int diff = d - pixel;
        double distance = xDiff2 + yDiff2 + diff * diff * pixelMult;
        if (distance < minDistance) {
          minDistance = distance;
        }
      }
    }
  }
  return col >= d ? -std::sqrt(minDistance) : std::sqrt(minDistance);
}

static float computeEuclideanDistance(int xp, int yp, QImage &image, int col,
                                      int d) {
  int width = image.width();
  int height = image.height();
  int minDistance = width * width + height * height;
  for (int y = yp; y < height; y++) {
    int yDiff2 = (yp - y) * (yp - y);
    uchar *inputRow = image.scanLine(y);
    for (int x = xp; x < width; x++) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }
    for (int x = xp - 1; x >= 0; x--) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }

    if (yDiff2 > minDistance) {
      break;
    }
  }

  for (int y = yp - 1; y >= 0; y--) {
    int yDiff2 = (yp - y) * (yp - y);
    uchar *inputRow = image.scanLine(y);
    for (int x = xp; x < width; x++) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }
    for (int x = xp - 1; x >= 0; x--) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }

    if (yDiff2 > minDistance) {
      break;
    }
  }

  return col >= d ? -std::sqrt(minDistance) : std::sqrt(minDistance);
}

static float **getDistanceField(QImage &image, int d, const double pixelMult) {
  int width = image.width();
  int height = image.height();
  float **output = allocFloatMatrix(width, height);

#pragma omp parallel for
  for (int y = 0; y < height; y++) {
    uchar *inputRow = image.scanLine(y);
    for (int x = 0; x < width; x++) {
      if (pixelMult >= 0.0) {
        output[y][x] =
            computeEuclideanDistance3D(x, y, image, inputRow[x], d, pixelMult);
      } else {
        output[y][x] = computeEuclideanDistance(x, y, image, inputRow[x], d);
      }
    }
  }
  return output;
}

QVector<float **> ImageView::calcDistanceField(QImage &image,
                                               double pixelMult) {
  resetProgress();
  int width = image.width();
  int height = image.height();
  int depth = image.depth();
  int dynRange = quantisized ? sampleSettings.quantisizeLevel : 1 << depth;
  QVector<float **> df;
  df.resize(dynRange);

  if (pixelMult >= 0.0) {
    for (int d = 0; d < dynRange; d++) {
      df[d] = getDistanceField(image, d, pixelMult);
      setProgress((d + 1) / static_cast<float>(dynRange) * 100);
    }
    finalizeProgress();
  } else {
    setProgress(1.0f / static_cast<float>(dynRange) * 100);

    for (int d = 1; d < dynRange; d++) {
      df[d] = getDistanceField(image, d, pixelMult);
      setProgress((d + 1) / static_cast<float>(dynRange) * 100);
    }

    float **l0 = allocFloatMatrix(width, height);
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        l0[y][x] = df[1][y][x] - 10;
      }
    }

    df[0] = l0;
    finalizeProgress();
  }
  //  QVector<float> vals;
  //  vals.reserve(dynRange);
  //  for (int y = 0; y < height; y++) {
  //    for (int x = 0; x < 2; x++) {
  //      vals.clear();
  //      for (int d = 0; d < dynRange; d++) {
  //        vals.append(df[d][y][x]);
  //      }
  //      qDebug() << vals;
  //    }
  //  }
  return df;
}
