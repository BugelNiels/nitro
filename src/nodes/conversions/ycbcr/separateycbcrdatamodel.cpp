#include "separateycbcrdatamodel.hpp"

nitro::SeparateYCbCrDataModel::SeparateYCbCrDataModel() : ColorSeparateDataModel(3) {

}

static inline uchar clampConvert(float a) {
    a += 0.5f;
    a = a < 0 ? 0 : a;
    a = a > 255 ? 255 : a;
    return uchar(a);
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
    y = clampConvert(fy);
    cb = clampConvert(fcb);
    cr = clampConvert(fcr);
}

void nitro::SeparateYCbCrDataModel::separate(const QImage &img) {
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
    _result[0] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(yChannel));
    _result[1] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(cbChannel));
    _result[2] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(crChannel));
}
