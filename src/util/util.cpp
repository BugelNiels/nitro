
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "util.hpp"
#include <QDebug>

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


QImage nitro::toQImage(const cv::Mat& im) {
    if (im.empty()) {
        return {};  // Return an empty QImage if the input image is empty
    }

    // Determine the image format and number of channels
    QImage::Format format;
    int numChannels = im.channels();
    if (numChannels == 1) {
        format = QImage::Format_Grayscale8;
    } else if (numChannels == 3) {
        format = QImage::Format_RGB888;
    } else {
        // Unsupported number of channels
        qDebug() << "Unsupported number of channels";
        return {};
    }
    cv::cvtColor(im, im, cv::COLOR_BGR2RGB);

    // Create a QImage with the appropriate format and dimensions
    QImage qImage(im.data, im.cols, im.rows, static_cast<int>(im.step), format);

    return qImage;
}

/*
 * Taken from https://stackoverflow.com/a/50848100
 */
cv::Mat toCvImage(const QImage& image) {
    cv::Mat out;
    switch(image.format()) {
        case QImage::Format_Invalid:
        {
            cv::Mat empty;
            empty.copyTo(out);
            break;
        }
        case QImage::Format_RGB32:
        {
            cv::Mat view(image.height(),image.width(),CV_8UC4,(void *)image.constBits(),image.bytesPerLine());
            view.copyTo(out);
            break;
        }
        case QImage::Format_RGB888:
        {
            cv::Mat view(image.height(),image.width(),CV_8UC3,(void *)image.constBits(),image.bytesPerLine());
            cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
            break;
        }
        default:
        {
            QImage conv = image.convertToFormat(QImage::Format_ARGB32);
            cv::Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
            view.copyTo(out);
            break;
        }
    }
    return out;
}