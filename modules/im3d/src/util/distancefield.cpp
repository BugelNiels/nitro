#include "distancefield.hpp"
#include <opencv2/imgproc.hpp>
#include <stack>
#include <QDebug>

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

static cv::Mat dtMeijsterRoerdinkHesselink(int foreground,
                                           const cv::Mat &image,
                                           bool inside) {
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
            while ((q >= 0) && ((t[q] - s[q]) * (t[q] - s[q]) + vsq >
                                (t[q] - x) * (t[q] - x) + vxy)) {
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

typedef struct item {
    int val;
    int idx;
} item;

static std::vector<std::vector<item>> maskDistTransform(const cv::Mat &image) {
    int cols = image.cols;
    int rows = image.rows;
    int infinity = rows + cols + 1;

    // TODO: allocate once
    std::vector<item> temp(rows);
    std::vector<item> par(rows);
    std::vector<item> stack(256);


    std::vector<std::vector<item>> dt(cols);
    /* top-down */
    for (int x = 0; x < cols; x++) {
        int stackIdx = 0;
        stack[stackIdx] = {-1, -1};
        stack[++stackIdx] = {image.at<uchar>(0, x), 0};
        par[0] = stack[stackIdx];
        for (int y = 1; y < rows; y++) {
            uchar pixel = image.at<uchar>(y, x);
            int curLevel = stack[stackIdx].val;
            if (pixel == curLevel) {
                par[y] = stack[stackIdx];
            } else if (pixel > curLevel) {
                par[y] = stack[stackIdx];
                stackIdx++;
                stack[stackIdx] = {pixel, y};
            } else { // pixel <
                while (stackIdx > 0 && pixel < curLevel) {
                    stackIdx--;
                    curLevel = stack[stackIdx].val;
                }

                if (pixel == curLevel) {
                    par[y] = stack[stackIdx];
                } else {
                    par[y] = par[stack[stackIdx].idx];
                    stack[++stackIdx] = {pixel, y};
                }
            }
        }
        temp = par;
        // Bottom up
        stackIdx = 0;
        stack[stackIdx] = {-1, -1};
        stack[stackIdx++] = {image.at<uchar>(rows - 1, x), rows - 1};
        par[rows - 1] = stack[stackIdx];
        for (int y = rows - 2; y >= 0; y--) {
            uchar pixel = image.at<uchar>(y, x);
            int curLevel = stack[stackIdx].val;
            if (pixel == curLevel) {
                par[y] = stack[stackIdx];
            } else if (pixel > curLevel) {
                par[y] = stack[stackIdx];
                stackIdx++;
                stack[stackIdx] = {pixel, y};
            } else { // pixel <
                while (stackIdx > 0 && pixel < curLevel) {
                    stackIdx--;
                    curLevel = stack[stackIdx].val;
                }

                if (pixel == curLevel) {
                    par[y] = stack[stackIdx];
                } else {
                    par[y] = par[stack[stackIdx].idx];
                    stackIdx++;
                    stack[stackIdx] = {pixel, y};
                }
            }
        }
        dt[x].resize(rows);
        for (int y = 0; y < rows; y++) {
            item topDownI = temp[y];
            item bottomUpI = par[y];
            // TODO: put the actual dist values here
            item lowest = topDownI.val < bottomUpI.val ? topDownI : bottomUpI;
            lowest.val = lowest.val < rows ? lowest.val * lowest.val : infinity;
            dt[x][y] = lowest;
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

static int getDist(const cv::Mat &src, const std::vector<std::vector<item>> &vdt, int x, int y, int layer) {
    int pixel = src.at<uchar>(y, x);
    int y2 = y;
    while (pixel >= layer) {
        int newY = vdt[x][y2].idx;
        if (newY == y2) {
            return 14235;
        }
        y2 = newY;
        pixel = src.at<uchar>(y2, x);
    }
    return vdt[x][y2].val + y - y2;
}

static std::vector<cv::Mat> distFields(const cv::Mat &image, int numLevels) {
    int cols = image.cols;
    int rows = image.rows;
    int infinity = cols * cols + rows * rows;  // or anything larger than this

    /* vertical phase */
    auto vdt = maskDistTransform(image);
    exit(0);

    std::vector<cv::Mat> df(numLevels);
    std::vector<int> s(cols);
    std::vector<int> t(cols);
    for (int d = 0; d < numLevels; d++) {
        /* horizontal phase */
        cv::Mat dt(rows, cols, CV_32FC1);
        for (int y = 0; y < rows; y++) {
            /* left-to-right scan */
            int q = 0;
            s[0] = 0;
            t[0] = 0;

            float *dtRow = dt.ptr<float>(y);
            for (int x = 1; x < cols; x++) {
                int vsq = getDist(image, vdt, s[q], y, d);
                int vxy = getDist(image, vdt, x, y, d);
                while ((q >= 0) && ((t[q] - s[q]) * (t[q] - s[q]) + vsq >
                                    (t[q] - x) * (t[q] - x) + vxy)) {
                    q--;
                    if (q >= 0) vsq = getDist(image, vdt, s[q], y, d);
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
            int vsq = getDist(image, vdt, s[q], y, d);
            for (int x = cols - 1; x >= 0; x--) {
                dtRow[x] = std::sqrt((x - s[q]) * (x - s[q]) + vsq);
                if (x == t[q]) {
                    q--;
                    if (q >= 0) {
                        vsq = getDist(image, vdt, s[q], y, d);
                    }
                }
            }
        }
        df[d] = dt;
    }
    return df;
}

// Expects a uchar input image
cv::Mat nitro::signedDistField(const cv::Mat &img, int val) {
    cv::Mat outside = dtMeijsterRoerdinkHesselink(val, img, false);
    cv::Mat inside = dtMeijsterRoerdinkHesselink(val, img, true);

    return outside - inside;
}


std::vector<cv::Mat> nitro::signedDistFields(const cv::Mat &img, int numLevels) {
    auto outside = distFields(img, numLevels);
    auto inside = distFields(numLevels - img, numLevels);

    for (int d = 0; d < numLevels; d++) {
        outside[d] -= inside[d];
    }

    return outside;
}
