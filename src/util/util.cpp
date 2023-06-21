
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "util.hpp"
#include <QDebug>
#include <iostream>

QColor nitro::makeReadable(const QColor &color, bool lightMode) {
    // Convert to YIQ color space
    double y = 0.299 * color.redF() + 0.587 * color.greenF() + 0.114 * color.blueF();
    double i = 0.596 * color.redF() - 0.274 * color.greenF() - 0.322 * color.blueF();
    double q = 0.211 * color.redF() - 0.523 * color.greenF() + 0.312 * color.blueF();

    // Adjust Y value to make color more readable on dark background
    if (lightMode) {
        y = qMin(0.4, y);
    } else {
        y = qMax(0.5, y);
    }

    // Convert back to RGB color space
    double r = y + 0.956 * i + 0.621 * q;
    double g = y - 0.272 * i - 0.647 * q;
    double b = y - 1.105 * i + 1.702 * q;

    // Clamp values to valid range
    r = qBound(0.0, r, 1.0);
    g = qBound(0.0, g, 1.0);
    b = qBound(0.0, b, 1.0);

    // Return adjusted color
    return QColor::fromRgbF(r, g, b, color.alphaF());
}


int nitro::getMaxValue(const cv::Mat &mat) {
    int depth = mat.depth();

    int maxValue;

    switch (depth) {
        case CV_8U:  // 8-bit unsigned integer (0-255)
            maxValue = 255;
            break;

        case CV_8S:  // 8-bit signed integer (-128 to 127)
            maxValue = 127;
            break;

        case CV_16U:  // 16-bit unsigned integer (0-65535)
            maxValue = 65535;
            break;

        case CV_16S:  // 16-bit signed integer (-32768 to 32767)
            maxValue = 32767;
            break;

        case CV_32S:  // 32-bit signed integer
            maxValue = INT_MAX;
            break;

        case CV_32F:  // 32-bit floating-point
        case CV_64F:  // 64-bit floating-point
            maxValue = 1;
            break;

        default:
            maxValue = 0;
            break;
    }
    return maxValue;
}


// Source for the next two functions: https://github.com/asmaloney/asmOpenCV/blob/master/asmOpenCV.h
QImage nitro::cvMatToQImage(const cv::Mat &src, cv::Mat &img) {

    switch (src.type()) {
        case CV_32F: {
            src.convertTo(img, CV_8U, 255);
            break;
        }
        case CV_32FC3: {
            src.convertTo(img, CV_8UC3, 255);
            break;
        }

        case CV_32FC4: {
            src.convertTo(img, CV_8UC4, 255);
            break;
        }
        default:
            src.copyTo(img);
            break;
    }

    switch (img.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            return QImage(img.data,
                          img.cols, img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_ARGB32);
        }

            // 8-bit, 3 channel
        case CV_8UC3: {
            return QImage(img.data,
                          img.cols, img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_RGB888);
        }

            // 8-bit, 1 channel
        case CV_8UC1: {
            return QImage(img.data,
                          img.cols, img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_Grayscale8);
        }

        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << img.type();
            break;
    }

    return {};
}

// If inImage exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inImage's
// data with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
//    NOTE: This does not cover all cases - it should be easy to add new ones as required.
inline cv::Mat nitro::QImageToCvMat(const QImage &inImage, bool inCloneImageData = true) {
    switch (inImage.format()) {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(inImage.height(), inImage.width(),
                        CV_8UC4,
                        const_cast<uchar *>(inImage.bits()),
                        static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

            // 8-bit, 3 channel
        case QImage::Format_RGB32: {
            if (!inCloneImageData) {
                qWarning()
                        << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
            }

            cv::Mat mat(inImage.height(), inImage.width(),
                        CV_8UC4,
                        const_cast<uchar *>(inImage.bits()),
                        static_cast<size_t>(inImage.bytesPerLine())
            );

            cv::Mat matNoAlpha;

            cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

            return matNoAlpha;
        }

            // 8-bit, 3 channel
        case QImage::Format_RGB888: {
            if (!inCloneImageData) {
                qWarning()
                        << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
            }

            QImage swapped = inImage.rgbSwapped();

            return cv::Mat(swapped.height(), swapped.width(),
                           CV_8UC3,
                           const_cast<uchar *>(swapped.bits()),
                           static_cast<size_t>(swapped.bytesPerLine())
            ).clone();
        }

            // 8-bit, 1 channel
        case QImage::Format_Indexed8: {
            cv::Mat mat(inImage.height(), inImage.width(),
                        CV_8UC1,
                        const_cast<uchar *>(inImage.bits()),
                        static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
            qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
    }

    return cv::Mat();
}

static bool equal(const cv::Mat &a, const cv::Mat &b) {
    if ((a.rows != b.rows) || (a.cols != b.cols))
        return false;
    cv::Scalar s = sum(a - b);
    return (s[0] == 0) && (s[1] == 0) && (s[2] == 0);
}

bool nitro::isGrayscale(const cv::Mat &img) {
    if (img.channels() > 1) {
        std::vector<cv::Mat> channels;
        cv::split(img, channels);
        for (int i = 0; i < channels.size(); i++) {
            if (!equal(channels[0], channels[i])) {
                return false;
            }
        }
        return true;
    }
    return true;
}


cv::Mat nitro::cropToMatchSize(const cv::Mat &srcImage, const cv::Mat &targetImage) {
    int targetWidth = targetImage.cols;
    int targetHeight = targetImage.rows;

    int cropWidth = std::min(targetWidth, srcImage.cols);
    int cropHeight = std::min(targetHeight, srcImage.rows);

    cv::Rect croppedRect(0, 0, cropWidth, cropHeight);

    cv::Mat canvas;
    targetImage.copyTo(canvas);
    srcImage(croppedRect).copyTo(canvas(croppedRect));
    return canvas;
}

cv::Mat nitro::createMask(const cv::MatSize &srcSize, const cv::MatSize &targetSize) {
    int targetWidth = targetSize[1];
    int targetHeight = targetSize[0];

    int cropWidth = std::min(targetWidth, srcSize[1]);
    int cropHeight = std::min(targetHeight, srcSize[0]);

    cv::Rect croppedRect(0, 0, cropWidth, cropHeight);

    cv::Mat mask(targetHeight, targetWidth, CV_8UC1, cv::Scalar(0));
    mask(croppedRect).setTo(cv::Scalar(1));
    return mask;
}
