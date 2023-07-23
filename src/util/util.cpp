#include <util.hpp>

#include <QDebug>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro {

QColor makeReadable(const QColor &color, bool lightMode) {
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

int getMaxValue(const cv::Mat &mat) {
    int depth = mat.depth();

    int maxValue;

    switch (depth) {
        case CV_8U: // 8-bit unsigned integer (0-255)
            maxValue = 255;
            break;

        case CV_8S: // 8-bit signed integer (-128 to 127)
            maxValue = 127;
            break;

        case CV_16U: // 16-bit unsigned integer (0-65535)
            maxValue = 65535;
            break;

        case CV_16S: // 16-bit signed integer (-32768 to 32767)
            maxValue = 32767;
            break;

        case CV_32S: // 32-bit signed integer
            maxValue = INT_MAX;
            break;

        case CV_32F: // 32-bit floating-point
        case CV_64F: // 64-bit floating-point
            maxValue = 1;
            break;

        default:
            maxValue = 0;
            break;
    }
    return maxValue;
}

// Source for the next two functions: https://github.com/asmaloney/asmOpenCV/blob/master/asmOpenCV.h
QImage cvMatToQImage(const cv::Mat &src, cv::Mat &img) {

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
                          img.cols,
                          img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_ARGB32);
        }

            // 8-bit, 3 channel
        case CV_8UC3: {
            return QImage(img.data,
                          img.cols,
                          img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_RGB888);
        }

            // 8-bit, 1 channel
        case CV_8UC1: {
            return QImage(img.data,
                          img.cols,
                          img.rows,
                          static_cast<int>(img.step),
                          QImage::Format_Grayscale8);
        }

        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:"
                       << img.type();
            break;
    }

    return {};
}

static bool equal(const cv::Mat &a, const cv::Mat &b) {
    if ((a.rows != b.rows) || (a.cols != b.cols))
        return false;
    cv::Scalar s = sum(a - b);
    return (s[0] == 0) && (s[1] == 0) && (s[2] == 0);
}

bool isGrayscale(const cv::Mat &img) {
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

cv::Mat resize(const cv::Mat &imIn,
               const cv::Size &targetSize,
               const cv::InterpolationFlags mode,
               AspectRatioMode arMode) {
    cv::Mat result;

    switch (arMode) {

        case AspectRatioMode::IGNORE:
            cv::resize(imIn, result, targetSize, 0, 0, mode);
            break;
        case AspectRatioMode::KEEP_CROP: {
            int inWidth = imIn.cols;
            int inHeight = imIn.rows;
            double aspectRatio = double(inWidth) / double(inHeight);
            int targetWidth = targetSize.width;
            int targetHeight = targetSize.height;
            double targetRatio = double(targetWidth) / double(targetHeight);

            int cropWidth = inWidth;
            int cropHeight = inHeight;
            if (aspectRatio > targetRatio) {
                // need to crop the width
                cropWidth = targetRatio * inHeight;
            } else {
                // need to crop the height
                cropHeight = inWidth / targetRatio;
            }
            cropWidth = std::max(cropWidth, 1);
            cropHeight = std::max(cropHeight, 1);
            cv::Rect croppedRect(0, 0, cropWidth, cropHeight);
            cv::resize(imIn(croppedRect), result, targetSize, 0, 0, mode);
            break;
        }
        case AspectRatioMode::KEEP_SHRINK: {
            cv::Size newSize = targetSize;
            double arIn = double(imIn.cols) / double(imIn.rows);
            double arTarget = double(targetSize.width) / double(targetSize.height);
            if (arIn > 1) {
                if (arIn > arTarget) {
                    newSize.height = int(std::round(newSize.width / arIn));
                } else {
                    newSize.width = int(std::round(newSize.height * arIn));
                }
            } else {
                if (arIn > arTarget) {
                    newSize.height = int(std::round(newSize.width / arIn));
                } else {
                    newSize.width = int(std::round(newSize.height * arIn));
                }
            }
            cv::resize(imIn, result, newSize, 0, 0, mode);
        } break;

        case AspectRatioMode::KEEP_GROW: {
            cv::Size newSize = targetSize;
            double arIn = double(imIn.cols) / double(imIn.rows);
            double arTarget = double(targetSize.width) / double(targetSize.height);
            if (arIn > 1) {
                if (arIn > arTarget) {
                    newSize.width = int(std::round(newSize.height * arIn));
                } else {
                    newSize.height = int(std::round(newSize.width / arIn));
                }
            } else {
                if (arIn > arTarget) {
                    newSize.width = int(std::round(newSize.height * arIn));
                } else {
                    newSize.height = int(std::round(newSize.width / arIn));
                }
            }
            cv::resize(imIn, result, newSize, 0, 0, mode);
        } break;
    }
    return result;
}

} // namespace nitro
