#include "imgconvert.hpp"

#include <QDebug>

float nitro::mse(const QImage &a, const QImage &b) {
    int width = a.width();
    int height = a.height();

    int err = 0;
    for (int y = 0; y < height; y++) {
        const uchar *aP = a.scanLine(y);
        const uchar *bP = b.scanLine(y);
        for (int x = 0; x < width; x++) {
            int diff = (aP[x] - bP[x]);
            err += std::abs(diff);
        }
    }
    return err / float(width * height);
}

static inline uchar clampConvert(float a) {
    a += 0.5f;
    a = a < 0 ? 0 : a;
    a = a > 255 ? 255 : a;
    return uchar(a);
}

void nitro::rgb2YCbCr(int r, int g, int b, uchar &y, uchar &cb, uchar &cr) {
    float fr = float(r);
    float fg = float(g);
    float fb = float(b);
    float fy = 16.0f + 65.738f * fr / 256.0f + 129.057f * fg / 256.0f +
               25.064f * fb / 256.0f;
    float fcb = 128.0f - 37.945f * fr / 256.0f - 74.494f * fg / 256.0f +
                112.439f * fb / 256.0f;
    float fcr = 128.0f + 112.493f * fr / 256.0f - 94.154f * fg / 256.0f -
                18.285f * fb / 256.0f;
    y = clampConvert(fy);
    cb = clampConvert(fcb);
    cr = clampConvert(fcr);
}

void nitro::yCbCr2Rgb(int y, int cb, int cr, uchar &r, uchar &g, uchar &b) {
    float fy = float(y);
    float fcb = float(cb);
    float fcr = float(cr);
    float fr = 298.082f * fy / 256.0f + 408.583f * fcr / 256.0f - 222.921f;
    float fg = 298.082f * fy / 256.0f - 100.291f * fcb / 256.0f -
               208.120f * fcr / 256.0f + 135.576f;
    float fb = 298.082f * fy / 256.0f + 516.412f * fcb / 256.0f - 276.836f;
    r = clampConvert(fr);
    g = clampConvert(fg);
    b = clampConvert(fb);
}

QVector<QImage> nitro::separateYCbCr(const QImage &img) {
    int width = img.width();
    int height = img.height();
    QImage yChannel(width, height, QImage::Format_Grayscale8);
    QImage cbChannel(width, height, QImage::Format_Grayscale8);
    QImage crChannel(width, height, QImage::Format_Grayscale8);

    for (int y = 0; y < height; y++) {
        uchar *yRow = yChannel.scanLine(y);
        uchar *cbRow = cbChannel.scanLine(y);
        uchar *crRow = crChannel.scanLine(y);
        for (int x = 0; x < width; x++) {
            QColor col = img.pixelColor(x, y);
            rgb2YCbCr(col.red(), col.green(), col.blue(), yRow[x], cbRow[x],
                      crRow[x]);
        }
    }
    return {yChannel, cbChannel, crChannel};
}

QImage nitro::combineYCbCr(const QVector<QImage> &imgs) {
    int width = imgs[0].width();
    int height = imgs[0].height();
    QImage colorImg(width, height, QImage::Format_RGB32);
    QImage yChannel = imgs[0];
    QImage cbChannel = imgs[1];
    QImage crChannel = imgs[2];

    for (int y = 0; y < height; y++) {
        uchar *yRow = yChannel.scanLine(y);
        uchar *cbRow = cbChannel.scanLine(y);
        uchar *crRow = crChannel.scanLine(y);
        for (int x = 0; x < width; x++) {
            uchar r, g, b;
            yCbCr2Rgb(yRow[x], cbRow[x], crRow[x], r, g, b);
            colorImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return colorImg;
}

QVector<QImage> nitro::separateRGB(const QImage &img) {
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

QImage nitro::combineRGB(const QVector<QImage> &imgs) {
    int width = imgs[0].width();
    int height = imgs[0].height();
    QImage colorImg(width, height, QImage::Format_RGB32);
    QImage red = imgs[0];
    QImage green = imgs[1];
    QImage blue = imgs[2];

    for (int y = 0; y < height; y++) {
        uchar *redRow = red.scanLine(y);
        uchar *greenRow = green.scanLine(y);
        uchar *blueRow = blue.scanLine(y);
        for (int x = 0; x < width; x++) {
            colorImg.setPixelColor(x, y, QColor(redRow[x], greenRow[x], blueRow[x]));
        }
    }
    return colorImg;
}