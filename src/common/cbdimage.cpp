#include "cbdimage.hpp"

#include <QDebug>
#include <iostream>
#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>

#include "resamplers/resampler.hpp"

#define USE_DIST

nitro::CbdImage::CbdImage() = default;

nitro::CbdImage::CbdImage(const nitro::CbdImage &img) {
    numGreyLevels = img.numGreyLevels;
    matrix = nitro::Matrix<int>(img.matrix);
    distanceField = img.distanceField;

    displayImg = img.displayImg;
    displayCalculated = img.displayCalculated;

    indexed = img.indexed;
    vals = img.vals;
}

nitro::CbdImage::CbdImage(int width, int height, int dynRange)
        : numGreyLevels(dynRange) {
    matrix = nitro::Matrix<int>(width, height);
}

nitro::CbdImage::CbdImage(const QImage &img) {
    int width = img.width();
    int height = img.height();
    matrix = nitro::Matrix<int>(width, height);
    numGreyLevels = 1 << img.depth();
    for (int y = 0; y < height; y++) {
        const uchar *inputRow = img.constScanLine(y);
        for (int x = 0; x < width; x++) {
            int val = inputRow[x];
            matrix.set(x, y, val);
        }
    }
}

nitro::CbdImage nitro::CbdImage::resample(int numDesiredLevels,
                                          const std::shared_ptr<Resampler> &resampler) {
    return resampler->resample(*this, numDesiredLevels);
}

int nextLevel(int i, const QVector<bool> &activeLevels) {
    int levels = activeLevels.size();
    int next = i + 1;
    while (!activeLevels[next] && next < levels - 1) next++;
    return next;
}

int prevLevel(int i, const QVector<bool> &activeLevels) {
    int prev = i - 1;
    while (!activeLevels[prev] && prev > 0) prev--;
    return prev;
}

float diff(nitro::Matrix<float> &a, nitro::Matrix<float> &b) {
    if (a.numElems() != b.numElems()) {
        return -1;
    }
    float err = 0;
    for (int i = 0; i < a.numElems(); i++) {
        err += std::abs(a.get(i) - b.get(i));
    }
    return err;
}

int diff(nitro::Matrix<int> &a, nitro::Matrix<int> &b) {
    if (a.numElems() != b.numElems()) {
        return -1;
    }
    int err = 0;
    for (int i = 0; i < a.numElems(); i++) {
        err += std::abs(a.get(i) - b.get(i));
    }
    return err;
}

int nitro::CbdImage::diff(CbdImage &other) {
    int err = 0;
    for (int i = 0; i < matrix.numElems(); i++) {
        int valA = indexed ? vals[matrix.get(i)] : matrix.get(i);
        int valB =
                other.indexed ? other.vals[other.matrix.get(i)] : other.matrix.get(i);
        err += std::abs(valA - valB);
    }
    return err;
}

int diffInt(nitro::Matrix<int> &a, nitro::Matrix<int> &b) {
    if (a.numElems() != b.numElems()) {
        qDebug() << "err";
        return -1;
    }
    int err = 0;
    for (int i = 0; i < a.numElems(); i++) {
        err += std::abs(a.get(i) - b.get(i));
    }
    return err;
}

int count(nitro::Matrix<int> &a, int val) {
    int err = 0;
    for (int i = 0; i < a.numElems(); i++) {
        err += a.get(i) != val;
    }
    return err;
}

typedef struct ErrorItem {
    int idx;
    int err;
} ErrorItem;

nitro::CbdImage nitro::CbdImage::compress(int targetBitDepth,
                                          const std::shared_ptr<Resampler> &resampler,
                                          ProgressUpdater *updater) {
    if (distanceField.empty()) {
        calcDistanceField();
        qDebug() << "Distance field calculated";
    }
    QVector<bool> activeLevels(numGreyLevels);
    activeLevels.fill(true);
    int imWidth = width();
    int imHeight = height();

    int targetNumLevels = 1 << targetBitDepth;

    CbdImage tImg = CbdImage(*this);
    tImg.indexed = true;
    tImg.vals.resize(numGreyLevels);
    for (int i = 0; i < numGreyLevels; i++) {
        if (indexed) {
            tImg.vals[i] = vals[i];
        } else {
            tImg.vals[i] = i;
        }
    }

    int currentSize = numGreyLevels;

    qDebug() << "Starting layer removal";
    qDebug() << targetNumLevels << currentSize;
    while (currentSize > targetNumLevels) {
        // Remove one level at a time and check which one is optimal
        ErrorItem lowestErr = {-1, -1};
        tImg.numGreyLevels--;
        for (int i = 0; i < numGreyLevels; i++) {
            if (activeLevels[i]) {
                int removedVal = tImg.vals[i];
                tImg.vals[i] = -1;
                CbdImage reconstructed = resampler->resample(tImg, numGreyLevels);

                int error = diff(reconstructed);
                if (lowestErr.err > error || lowestErr.idx == -1) {
                    lowestErr = {i, error};
                }
                tImg.vals[i] = removedVal;
            }
        }
        updater->addProgress(100.0f / float(numGreyLevels - targetNumLevels));
        activeLevels[lowestErr.idx] = false;
        tImg.vals[lowestErr.idx] = -1;
        currentSize--;
    }
    qDebug() << "Layer removal done";

    QVector<int> indices;
    indices.reserve(targetNumLevels);
    for (int i = 0; i < numGreyLevels; i++) {
        if (activeLevels[i]) {
            indices.append(i);
        }
    }
    qDebug() << indices;

    // Synthesize image from remaining distance fields

    CbdImage compressedImg(imWidth, imHeight, targetNumLevels);
    auto &compressedData = compressedImg.data();
    int numGreyLevelsFinal = indices.size();
    for (int y = 0; y < imHeight; y++) {
        for (int x = 0; x < imWidth; x++) {
            for (int d = numGreyLevelsFinal - 1; d >= 0; d--) {
                if (distanceField[indices[d]].get(x, y) <= 1) {
                    compressedData.set(x, y, d);
                    break;
                }
            }
        }
    }

    // set indices correctly
    compressedImg.vals.resize(indices.size());
    for (int i = 0; i < indices.size(); i++) {
        if (indexed) {
            compressedImg.vals[i] = vals[indices[i]];
        } else {
            compressedImg.vals[i] = indices[i];
        }
    }
    compressedImg.indexed = true;
    compressedImg.numGreyLevels = numGreyLevelsFinal;

    qDebug() << compressedImg.vals << numGreyLevelsFinal;

    // return image
    return compressedImg;
}

// CbdImage CbdImage::compress(int targetBitDepth) {
//   if (distanceField.size() == 0) {
//     calcDistanceField();
//   }
//   QVector<bool> activeLevels(numGreyLevels);
//   activeLevels.fill(true);
//   QVector<ErrorItem> errs(numGreyLevels);
//   int imWidth = width();
//   int imHeight = height();

//#ifndef USE_DIST
//  QVector<nitro::Matrix<int>> thresholds(numGreyLevels);
//  for (int i = 0; i < numGreyLevels; i++) {
//    nitro::Matrix<int> tMat(imWidth, imHeight);
//    for (int y = 0; y < imHeight; y++) {
//      for (int x = 0; x < imWidth; x++) {
//        tMat.set(x, y, matrix.get(x, y) <= i);
//      }
//    }
//    thresholds[i] = tMat;
//  }
//#endif

//  for (int i = 1; i < numGreyLevels - 1; i++) {
//    // TODO: reverse for-loop? We are better able to distinguish in the lower
//    // dynamic range part
//#ifndef USE_DIST
//    errs[i] = {i, diffInt(thresholds[i - 1], thresholds[i]) +
//                      diffInt(thresholds[i], thresholds[i + 1])};
//#else
//    errs[i] = {i, diff(distanceField[i - 1], distanceField[i]) +
//                      diff(distanceField[i], distanceField[i + 1])};
//#endif
//  }
//#ifdef USE_DIST

//  errs[0] = {0, diff(distanceField[0], distanceField[1])};
//  errs[numGreyLevels - 1] = {
//      numGreyLevels - 1,
//      diff(distanceField[numGreyLevels - 1], distanceField[numGreyLevels -
//      2])};
//#else
//  errs[0] = {0, diffInt(thresholds[0], thresholds[1])};
//  errs[numGreyLevels - 1] = {
//      numGreyLevels - 1,
//      diffInt(thresholds[numGreyLevels - 1], thresholds[numGreyLevels - 2])};
//#endif
//  std::sort(
//      errs.begin(), errs.end(),
//      [](const ErrorItem &a, const ErrorItem &b) { return a.err < b.err; });

//  int targetNumLevels = 1 << targetBitDepth;
//  int numLevelsToRemove = numGreyLevels - targetNumLevels;
//  int removed = 0;
//  for (int i = 0; i < numLevelsToRemove; i++) {
//    //    if (errs[i].err > 0) {
//    //      break;
//    //    }
//    removed++;
//    int levelToRemove = errs[i].idx;
//    activeLevels[levelToRemove] = false;
//  }
//  // construct list of active levels
//  QVector<int> indices;
//  //  indices.reserve(targetNumLevels);
//  indices.reserve(numGreyLevels - removed);
//  for (int i = 0; i < numGreyLevels; i++) {
//    if (activeLevels[i]) {
//      indices.append(i);
//    }
//  }

//  // Synthesize image from remaining distance fields

//  CbdImage compressedImg(imWidth, imHeight, targetNumLevels);
//  auto &compressedData = compressedImg.data();
//  int numGreyLevelsFinal = indices.size();
//  for (int y = 0; y < imHeight; y++) {
//    for (int x = 0; x < imWidth; x++) {
//      for (int d = numGreyLevelsFinal - 1; d >= 0; d--) {
//#ifdef USE_DIST
//        if (distanceField[indices[d]].get(x, y) <= 1) {
//          compressedData.set(x, y, d);
//        }
//#else
//        if (thresholds[indices[d]].get(x, y) == 1) {
//          compressedData.set(x, y, d);
//        }
//#endif
//      }
//    }
//  }

//  // set indices correctly
//  compressedImg.vals = indices;
//  compressedImg.indexed = true;
//  compressedImg.numGreyLevels = numGreyLevelsFinal;

//  qDebug() << indices;
//  // return image
//  return compressedImg;
//}

nitro::CbdImage nitro::CbdImage::decompress(int targetBitDepth,
                                            const std::shared_ptr<Resampler> &resampler) {
    if (distanceField.empty()) {
        calcDistanceField();
        qDebug() << "Distance field done";
    }
    return resample(1 << targetBitDepth, resampler);
}

static nitro::Matrix<int> maskDistanceTransform(int maskLength, int *maskDx,
                                                int *maskDy, int foreground,
                                                nitro::Matrix<int> &image, bool inside) {
    int width = image.width();
    int height = image.height();
    int infinity = width + height + 1;

    nitro::Matrix<int> dt(width, height);
    /* top-down, left-to-right pass */
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel = image.get(x, y);
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

static nitro::Matrix<int> dtMeijsterRoerdinkHesselink(int foreground,
                                                      nitro::Matrix<int> &image,
                                                      bool inside) {
    int width = image.width();
    int height = image.height();
    int infinity =
            width * width + height * height;  // or anything larger than this

    /* vertical phase */
    int maskDx[1] = {0};
    int maskDy[1] = {-1};
    nitro::Matrix<int> verticalDT =
            maskDistanceTransform(1, maskDx, maskDy, foreground, image, inside);

    /* square the verticalDT */
    // Can be parallelized
    nitro::Matrix<int> vdt(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int vdtval = verticalDT.get(x, y);
            vdtval = (vdtval < height ? vdtval * vdtval : infinity);
            vdt.set(x, y, vdtval);
        }
    }
    /* horizontal phase */
    nitro::Matrix<int> dt(width, height);
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

static nitro::Matrix<float> getDistanceFieldFast(nitro::Matrix<int> &image, int d) {
    int width = image.width();
    int height = image.height();
    nitro::Matrix<int> inside = dtMeijsterRoerdinkHesselink(d, image, true);
    nitro::Matrix<int> outside = dtMeijsterRoerdinkHesselink(d, image, false);
    nitro::Matrix<float> output(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (outside.get(x, y) == 0) {
                output.set(x, y, std::sqrt(inside.get(x, y)));
            } else {
                output.set(x, y, -std::sqrt(outside.get(x, y)));
            }
        }
    }
    return output;
}

void nitro::CbdImage::calcDistanceField(ProgressUpdater *updater) {
    int width = matrix.width();
    int height = matrix.height();
    QVector<nitro::Matrix<float>> df(numGreyLevels);

    //  if (pixelMult >= 0.0) {
    //    for (int d = 0; d < numGreyLevels; d++) {
    //      df[d] = getDistanceField(matrix, d, pixelMult);
    //    }
    //  } else {
    nitro::Matrix<float> l0(width, height);
#pragma omp parallel for
    for (int d = 1; d < numGreyLevels; d++) {
        df[d] = getDistanceFieldFast(matrix, d);
        if (updater != nullptr) {
            updater->addProgress(100 / float(numGreyLevels));
        }
    }

    // TODO: nowait here
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            l0.set(x, y, df[1].get(x, y) - 10);
        }
    }
    // TOOD: barrier here
    df[0] = l0;
    //  }

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
    distanceField = df;
}

QImage nitro::CbdImage::getDisplayImg() {
    if (displayCalculated) {
        return displayImg;
    }
    displayCalculated = true;
    int width = matrix.width();
    int height = matrix.height();

    displayImg = QImage(width, height, QImage::Format_Grayscale8);

    for (int y = 0; y < height; y++) {
        uchar *outputRow = displayImg.scanLine(y);
        for (int x = 0; x < width; x++) {
            if (indexed) {
                outputRow[x] = vals[matrix.get(x, y)];
            } else {
                outputRow[x] = matrix.get(x, y);
            }
        }
    }
    return displayImg;
}

void nitro::CbdImage::setIndexed(QVector<int> t_vals) {
    indexed = true;
    this->vals = t_vals;
}
