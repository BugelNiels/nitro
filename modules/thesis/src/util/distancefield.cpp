#include "distancefield.hpp"

#include <opencv2/imgproc.hpp>
#include <QDebug>

namespace nitro::Thesis {
static cv::Mat maskDistanceTransform(int foreground, const cv::Mat &image, bool inside) {
    int cols = image.cols;
    int rows = image.rows;
    int infinity = cols + rows + 1;

    cv::Mat dt(rows, cols, CV_32SC1);
    /* top-down, left-to-right pass */
    for (int y = 0; y < rows; y++) {
        const uchar *imRow = image.ptr<uchar>(y);
        int *dtRow = dt.ptr<int>(y);
        for (int x = 0; x < cols; x++) {
            uchar pixel = imRow[x];
            if ((inside && pixel >= foreground) || (!inside && pixel < foreground)) {
                dtRow[x] = 0;
            } else {
                int minnb = infinity;
                int nby = y - 1;
                if ((nby >= 0)) {
                    int nb = dt.at<int>(nby, x);
                    minnb = (minnb < nb ? minnb : nb);
                }

                minnb = (minnb < infinity ? 1 + minnb : infinity);
                dtRow[x] = minnb;
            }
        }
    }
    /* bottom-up, right-to-left pass */
    for (int y = rows - 1; y >= 0; y--) {
        int *dtRow = dt.ptr<int>(y);
        for (int x = cols - 1; x >= 0; x--) {
            int here = dtRow[x];
            if (here > 0) {
                int minnb = infinity;
                int nby = y + 1;
                if ((nby < rows)) {
                    int nb = dt.at<int>(nby, x);
                    minnb = (minnb < nb ? minnb : nb);
                }

                minnb = (minnb < infinity ? 1 + minnb : infinity);
                here = (minnb < here ? minnb : here);
                dtRow[x] = here;
            }
        }
    }
    return dt;
}

/** Euclidean distance transform ****************************************/

/* The Euclidean distance transform according to
 * Arnold Meijster, Jos BTM Roerdink, and Wim H Hesselink.
 * "A general algorithm for computing distance transforms
 * in linear time.", In: Mathematical Morphology and its
 * Applications to Image and Signal Processing, J. Goutsias,
 * L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340.
 */

/*********************** Distance dependant functions *********************/

static cv::Mat dtMeijsterRoerdinkHesselink(int foreground, const cv::Mat &image, bool inside) {
    int cols = image.cols;
    int rows = image.rows;
    int infinity = cols * cols + rows * rows;  // or anything larger than this

    /* vertical phase */
    cv::Mat vdt = maskDistanceTransform(foreground, image, inside);

    /* square the verticalDT */
    for (int y = 0; y < rows; y++) {
        int *vdtRow = vdt.ptr<int>(y);
        for (int x = 0; x < cols; x++) {
            int vdtval = vdtRow[x];
            vdtval = (vdtval < rows ? vdtval * vdtval : infinity);
            vdtRow[x] = vdtval;
        }
    }
    /* horizontal phase */
    cv::Mat dt(rows, cols, CV_64FC1);
    std::vector<int> s(cols);
    std::vector<int> t(cols);
    for (int y = 0; y < rows; y++) {
        /* left-to-right scan */
        int q = 0;
        s[0] = 0;
        t[0] = 0;

        double *dtRow = dt.ptr<double>(y);
        int *vdtRow = vdt.ptr<int>(y);
        for (int x = 1; x < cols; x++) {
            int vsq = vdtRow[s[q]];
            int vxy = vdtRow[x];
            while ((q >= 0) && ((t[q] - s[q]) * (t[q] - s[q]) + vsq > (t[q] - x) * (t[q] - x) + vxy)) {
                q--;
                if (q >= 0) vsq = vdtRow[s[q]];
            }
            if (q < 0) {
                q = 0;
                s[0] = x;
            } else {
                int w = 1 + (x * x - s[q] * s[q] + vxy - vsq) / (2 * (x - s[q]));
                if (w < cols) {
                    q++;
                    s[q] = x;
                    t[q] = w;
                }
            }
        }
        /* backward scan */
        int vsq = vdtRow[s[q]];
        for (int x = cols - 1; x >= 0; x--) {
            dtRow[x] = std::sqrt((x - s[q]) * (x - s[q]) + vsq);
            if (x == t[q]) {
                q--;
                if (q >= 0) {
                    vsq = vdtRow[s[q]];
                }
            }
        }
    }
    return dt;
}

// Expects a uchar input image
cv::Mat signedDistField(const cv::Mat &img, int val) {
    cv::Mat outside = dtMeijsterRoerdinkHesselink(val, img, false);
    cv::Mat inside = dtMeijsterRoerdinkHesselink(val, img, true);

    return outside - inside;
}
} // nitro::Thesis
