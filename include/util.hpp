#pragma once

#include <QImage>

namespace nitro {

    static inline uchar clampConvert(float a) {
        a += 0.5f;
        a = a < 0 ? 0 : a;
        a = a > 255 ? 255 : a;
        return uchar(a);
    }

    static inline uchar clamp(int a) {
        a = a < 0 ? 0 : a;
        a = a > 255 ? 255 : a;
        return uchar(a);
    }

    QColor makeReadable(const QColor &color, bool lightMode = false);

    QImage cvMatToQImage(const cv::Mat &inMat);

    cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData);

    int getMaxValue(const cv::Mat &mat);

    bool isGrayscale(const cv::Mat &img);
} // nitro
