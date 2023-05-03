
#include "ycbcrconvert.hpp"

#include "util/util.hpp"

static inline void yCbCr2Rgb(int y, int cb, int cr, uchar &r, uchar &g, uchar &b) {
    float fy = float(y);
    float fcb = float(cb);
    float fcr = float(cr);
    float fr = 298.082f * fy / 256.0f + 408.583f * fcr / 256.0f - 222.921f;
    float fg = 298.082f * fy / 256.0f - 100.291f * fcb / 256.0f -
               208.120f * fcr / 256.0f + 135.576f;
    float fb = 298.082f * fy / 256.0f + 516.412f * fcb / 256.0f - 276.836f;
    r = nitro::clampConvert(fr);
    g = nitro::clampConvert(fg);
    b = nitro::clampConvert(fb);
}


static inline void rgb2YCbCr(int r, int g, int b, uchar &y, uchar &cb, uchar &cr) {
    float fr = float(r);
    float fg = float(g);
    float fb = float(b);
    float fy = 16.0f + 65.738f * fr / 256.0f + 129.057f * fg / 256.0f +
               25.064f * fb / 256.0f;
    float fcb = 128.0f - 37.945f * fr / 256.0f - 74.494f * fg / 256.0f +
                112.439f * fb / 256.0f;
    float fcr = 128.0f + 112.493f * fr / 256.0f - 94.154f * fg / 256.0f -
                18.285f * fb / 256.0f;
    y = nitro::clampConvert(fy);
    cb = nitro::clampConvert(fcb);
    cr = nitro::clampConvert(fcr);
}

QVector<QImage> nitro::operations::separateYCbCr(const QImage &img) {
    int width = img.width();
    int height = img.height();
    // TODO: format for higher bit depths
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


QImage nitro::operations::combineYCbCr(const QImage &yChannel, const QImage &cbChannel, const QImage &crChannel) {

    int width = yChannel.width();
    int height = yChannel.height();
    QImage colorImg(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        const uchar *yRow = yChannel.constScanLine(y);
        const uchar *cbRow = cbChannel.constScanLine(y);
        const uchar *crRow = crChannel.constScanLine(y);
        for (int x = 0; x < width; x++) {
            uchar r, g, b;
            yCbCr2Rgb(yRow[x], cbRow[x], crRow[x], r, g, b);
            colorImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return colorImg;
}