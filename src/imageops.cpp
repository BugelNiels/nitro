#include <iostream>

#include "imageview.h"
#include "matrix.h"

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

QImage ImageView::compress(QImage &img, int targetBitDepth) {}

QImage ImageView::decompress(QImage &img, int targetBitDepth) {
  //  QVector<float **> compressedDF = distanceField;

  //  int width = img.width();
  //  int height = img.height();
  //  int depth = img.depth();
  //  int targetDynRange = pow(2, targetBitDepth) - 1;

  //  for (int y = 0; y < height; y++) {
  //    for (int x = 0; x < width; x++) {
  //      for (int d = targetDynRange; d >= 0; d--) {
  //        double dist =
  //            distFunc(LINEAR, sdfs, x, y, d, inputDynRange, targetDynRange);
  //        if (dist <= 0) {
  //          setIntPixel(&resampled, x, y, d);
  //          break;
  //        }
  //      }
  //    }
  //  }
}

void ImageView::compressDecompress() {
  resetProgress();
  QImage compressImg = compress(originalImg, 3);
  savedImages.append(compressImg);
  QImage decompressImg = decompress(originalImg, 4);
  finalizeProgress();
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
  //  return col >= d ? -minDistance : minDistance;
}

static Matrix<float> getDistanceField(QImage &image, int d,
                                      const double pixelMult) {
  int width = image.width();
  int height = image.height();
  Matrix<float> output(width, height);

#pragma omp parallel for
  for (int y = 0; y < height; y++) {
    uchar *inputRow = image.scanLine(y);
    for (int x = 0; x < width; x++) {
      if (pixelMult >= 0.0) {
        output.set(
            x, y,
            computeEuclideanDistance3D(x, y, image, inputRow[x], d, pixelMult));
      } else {
        output.set(x, y, computeEuclideanDistance(x, y, image, inputRow[x], d));
      }
    }
  }
  return output;
}

static Matrix<int> maskDistanceTransform(int maskLength, int *maskDx,
                                         int *maskDy, int foreground,
                                         QImage &im, bool inside) {
  int width = im.width();
  int height = im.height();
  int infinity = width + height + 1;

  Matrix<int> dt(width, height);
  /* top-down, left-to-right pass */
  for (int y = 0; y < height; y++) {
    uchar *inputRow = im.scanLine(y);
    for (int x = 0; x < width; x++) {
      int pixel = inputRow[x];
      if ((inside && pixel >= foreground) || (!inside && pixel < foreground)) {
        //      if (pixel <= foreground) {
        dt.set(x, y, 0);
      } else {
        int nb, minnb = infinity;
        for (int n = 0; n < maskLength; n++) {
          int nbx = x + maskDx[n];
          int nby = y + maskDy[n];
          if ((nbx >= 0) && (nbx < width) && (nby >= 0) && (nby < height)) {
            // this routine can be optimized quite a bit to make sure that
            // the above boundary checking is not needed for ech pixel.
            // feel free to optimize it.
            nb = dt.get(nbx, nby);
            minnb = (minnb < nb ? minnb : nb);
          }
        }
        minnb = (minnb < infinity ? 1 + minnb : infinity);
        dt.set(x, y, minnb);
      }
    }
  }
  /* bottom-up, right-to-left pass */
  for (int y = height - 1; y >= 0; y--) {
    for (int x = width - 1; x >= 0; x--) {
      int here = dt.get(x, y);
      if (here > 0) {
        int nb, minnb = infinity;
        for (int n = 0; n < maskLength; n++) {
          int nbx = x - maskDx[n];
          int nby = y - maskDy[n];
          if ((nbx >= 0) && (nbx < width) && (nby >= 0) && (nby < height)) {
            nb = dt.get(nbx, nby);
            minnb = (minnb < nb ? minnb : nb);
          }
        }
        minnb = (minnb < infinity ? 1 + minnb : infinity);
        here = (minnb < here ? minnb : here);
        dt.set(x, y, here);
      }
    }
  }
  return dt;
}

/** Euclidean distance transform ****************************************/

/* The Euclidean distance transform according to
 *Arnold Meijster, Jos BTM Roerdink, and Wim H Hesselink.
 *"A general algorithm for computing distance transforms
 *in linear time.", In: Mathematical Morphology and its
 *Applications to Image and Signal Processing, J. Goutsias,
 *L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340.
 */

/*********************** Distance dependant functions *********************/

static Matrix<int> dtMeijsterRoerdinkHesselink(int foreground, QImage &im,
                                               bool inside) {
  int width = im.width();
  int height = im.height();
  int infinity =
      width * width + height * height;  // or anything larger than this

  /* vertical phase */
  int maskDx[1] = {0};
  int maskDy[1] = {-1};
  Matrix<int> verticalDT =
      maskDistanceTransform(1, maskDx, maskDy, foreground, im, inside);

  /* square the verticalDT */
  // Can be parallelized
  Matrix<int> vdt(width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int vdtval = verticalDT.get(x, y);
      vdtval = (vdtval < height ? vdtval * vdtval : infinity);
      vdt.set(x, y, vdtval);
    }
  }
  /* horizontal phase */
  Matrix<int> dt(width, height);
  QVector<int> s(width);
  QVector<int> t(width);
  for (int y = 0; y < height; y++) {
    /* left-to-right scan */
    int q = 0;
    s[0] = 0;
    t[0] = 0;
    for (int x = 1; x < width; x++) {
      int vsq = vdt.get(s[q], y);
      int vxy = vdt.get(x, y);
      while ((q >= 0) && ((t[q] - s[q]) * (t[q] - s[q]) + vsq >
                          (t[q] - x) * (t[q] - x) + vxy)) {
        q--;
        if (q >= 0) vsq = vdt.get(s[q], y);
      }
      if (q < 0) {
        q = 0;
        s[0] = x;
      } else {
        int w = 1 + (x * x - s[q] * s[q] + vxy - vsq) / (2 * (x - s[q]));
        if (w < width) {
          q++;
          s[q] = x;
          t[q] = w;
        }
      }
    }
    /* backward scan */
    int vsq = vdt.get(s[q], y);
    for (int x = width - 1; x >= 0; x--) {
      dt.set(x, y, (x - s[q]) * (x - s[q]) + vsq);
      if (x == t[q]) {
        q--;
        if (q >= 0) vsq = vdt.get(s[q], y);
      }
    }
  }
  return dt;
}

static Matrix<float> getDistanceFieldFast(QImage &image, int d) {
  int width = image.width();
  int height = image.height();
  Matrix<int> inside = dtMeijsterRoerdinkHesselink(d, image, true);
  Matrix<int> outside = dtMeijsterRoerdinkHesselink(d, image, false);
  Matrix<float> output(width, height);

#pragma omp parallel for
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (outside.get(x, y) == 0) {
        output.set(x, y, std::sqrt(inside.get(x, y)));
      } else {
        output.set(x, y, -std::sqrt(outside.get(x, y)));
      }
    }
  }
  //  Matrix<float> test = getDistanceField(image, d, 1);
  //  for (int y = 0; y < height; y++) {
  //    for (int x = 0; x < width; x++) {
  //      std::cout << output[y][x] << " ";
  //    }
  //  }
  return output;
}

QVector<Matrix<float>> ImageView::calcDistanceField(QImage &image,
                                                    double pixelMult) {
  resetProgress();
  int width = image.width();
  int height = image.height();
  int depth = image.depth();
  int dynRange = quantisized ? sampleSettings.quantisizeLevel : 1 << depth;
  QVector<Matrix<float>> df;
  df.resize(dynRange);

  if (pixelMult >= 0.0) {
    for (int d = 0; d < dynRange; d++) {
      if (sampleSettings.fastSdf) {
        df[d] = getDistanceFieldFast(image, d);
      } else {
        df[d] = getDistanceField(image, d, pixelMult);
      }
      setProgress((d + 1) / static_cast<float>(dynRange) * 100);
    }
    finalizeProgress();
  } else {
    setProgress(1.0f / static_cast<float>(dynRange) * 100);

    for (int d = 1; d < dynRange; d++) {
      if (sampleSettings.fastSdf) {
        df[d] = getDistanceFieldFast(image, d);
      } else {
        df[d] = getDistanceField(image, d, pixelMult);
      }
      setProgress((d + 1) / static_cast<float>(dynRange) * 100);
    }

    Matrix<float> l0(width, height);
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        l0.set(x, y, df[1].get(x, y) - 10);
      }
    }

    df[0] = l0;
    finalizeProgress();
  }

  //  QImage distImg(width, height, QImage::Format_Grayscale8);
  //  int dfIdx = 1;
  //  float max = df[dfIdx].get(0, 0);
  //  for (int y = 0; y < height; y++) {
  //    for (int x = 0; x < width; x++) {
  //      float val = df[dfIdx].get(x, y);
  //      if (val < 0) {
  //        val *= -1;
  //      }
  //      max = MAX(max, val);
  //    }
  //  }
  //  for (int y = 0; y < height; y++) {
  //    uchar *inputRow = distImg.scanLine(y);
  //    for (int x = 0; x < width; x++) {
  //      float val = df[dfIdx].get(x, y);
  //      if (val < 0) {
  //        val *= -1;
  //      }
  //      inputRow[x] = val / (float)(max)*256;
  //    }
  //  }
  //  savedImages[sampleSettings.fastSdf ? 1 : 0] = distImg;
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
