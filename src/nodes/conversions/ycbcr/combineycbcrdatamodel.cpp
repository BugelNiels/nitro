#include "combineycbcrdatamodel.hpp"

nitro::CombineYCbrCrDataModel::CombineYCbrCrDataModel() : ColorCombineDataModel(3) {

}

// TODO: general util file
static inline uchar clampConvert(float a) {
    a += 0.5f;
    a = a < 0 ? 0 : a;
    a = a > 255 ? 255 : a;
    return uchar(a);
}

static inline void yCbCr2Rgb(int y, int cb, int cr, uchar &r, uchar &g, uchar &b) {
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

void nitro::CombineYCbrCrDataModel::combine(const QVector<std::shared_ptr<ImageData>> &input) {
    QImage yChannel = input[0]->getDisplayImg();
    QImage cbChannel = input[1]->getDisplayImg();
    QImage crChannel = input[2]->getDisplayImg();


    int width = yChannel.width();
    int height = yChannel.height();
    QImage colorImg(width, height, QImage::Format_RGB32);

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
    _result = std::make_shared<ImageData>(std::make_shared<QImage>(colorImg));
}
