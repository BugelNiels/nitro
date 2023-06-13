#pragma once

#include <QImage>

namespace nitro {

    QColor makeReadable(const QColor &color, bool lightMode = false);

    QImage cvMatToQImage(const cv::Mat &inMat);

    cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData);

    int getMaxValue(const cv::Mat &mat);

    bool isGrayscale(const cv::Mat &img);
} // nitro
