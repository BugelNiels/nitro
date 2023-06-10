#pragma once

#include <QImage>

namespace nitro {

    // TODO: check inline usage
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


    QImage toQImage(const cv::Mat &im);
    cv::Mat toCvImage(const QImage &im);
} // nitro
