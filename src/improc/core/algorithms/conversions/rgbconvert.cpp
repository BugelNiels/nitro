#include "rgbconvert.hpp"


QImage nitro::operations::combineRgb(const QImage &red, const QImage &green, const QImage &blue) {
    int width = red.width();
    int height = red.height();
    QImage colorImg(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        const uchar *redRow = red.constScanLine(y);
        const uchar *greenRow = green.constScanLine(y);
        const uchar *blueRow = blue.constScanLine(y);
        for (int x = 0; x < width; x++) {
            colorImg.setPixelColor(x, y, QColor(redRow[x], greenRow[x], blueRow[x]));
        }
    }
    return colorImg;
}

QVector<QImage> nitro::operations::separateRgb(const QImage &img) {
    int width = img.width();
    int height = img.height();
    QImage red(width, height, QImage::Format_Grayscale8);
    QImage green(width, height, QImage::Format_Grayscale8);
    QImage blue(width, height, QImage::Format_Grayscale8);

    for (int y = 0; y < height; y++) {
        uchar *redRow = red.scanLine(y);
        uchar *greenRow = green.scanLine(y);
        uchar *blueRow = blue.scanLine(y);
        for (int x = 0; x < width; x++) {
            QColor col = img.pixelColor(x, y);
            redRow[x] = col.red();
            greenRow[x] = col.green();
            blueRow[x] = col.blue();
        }
    }
    return {red, green, blue};
}
