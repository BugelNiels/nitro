#include "distancefield.hpp"


nitro::DistanceField::DistanceField() : dfWidth(0), dfHeight(0), numDfLevels(0) {

}

nitro::DistanceField::DistanceField(const nitro::CbdImage &img) : dfWidth(img.width()), dfHeight(img.height()),
                                                                  numDfLevels(img.numLevels()) {
    calcDistanceField(img);
    displayImages.resize(numDfLevels);
}


void nitro::DistanceField::calcDistanceField(const nitro::CbdImage &img) {
    auto &matrix = img.constData();
    QVector<nitro::Matrix<float>> df(numDfLevels);

    nitro::Matrix<float> l0(dfWidth, dfHeight);
#pragma omp parallel for default(none) shared(df, matrix)
    for (int d = 1; d < numDfLevels; d++) {
        df[d] = calcDistanceFieldFast(matrix, d);
    }

    if(numDfLevels > 1) {
        for (int y = 0; y < dfHeight; y++) {
            for (int x = 0; x < dfWidth; x++) {
                l0.set(x, y, df[1].get(x, y) - 10);
            }
        }
    } else {

        df[0] = calcDistanceFieldFast(matrix, 1);
    }
    // TOOD: barrier here
    df[0] = l0;

    distanceField = df;
}


static nitro::Matrix<int> maskDistanceTransform(int maskLength, int *maskDx,
                                                int *maskDy, int foreground,
                                                const nitro::Matrix<int> &image, bool inside) {
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
                                                      const nitro::Matrix<int> &image,
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

nitro::Matrix<float> nitro::DistanceField::calcDistanceFieldFast(const Matrix<int> &image, int d) {
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

const QImage &nitro::DistanceField::getDisplayImg(int index) {
    QImage &dImg = displayImages[index];
    if (dImg.width() == dfWidth && dImg.height() == dfHeight) {
        return dImg;
    }
    dImg = QImage(dfWidth, dfHeight, QImage::Format_Grayscale8);
    // find min and max
    auto df = distanceField[index];
    float min = dfWidth * dfHeight;
    float max = 0;
    for (int y = 0; y < dfHeight; y++) {
        for (int x = 0; x < dfWidth; x++) {
            min = std::min(df.get(x, y), min);
            max = std::max(df.get(x, y), max);
        }
    }
    // Adjust values
    for (int y = 0; y < dfHeight; y++) {
        auto *scanLine = dImg.scanLine(y);
        for (int x = 0; x < dfWidth; x++) {
            scanLine[x] = (df.get(x, y) - min) / (max - min) * 255;
        }
    }
    return dImg;
}

bool nitro::DistanceField::valid() const {
    return dfWidth > 0 && dfHeight > 0 && numDfLevels > 0;
}
