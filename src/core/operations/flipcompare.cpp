#include "flipcompare.hpp"

#include <FLIP.h>


static FLIP::image<FLIP::color3> qColImgToFlipImg(const QImage &img) {
    int width = img.width();
    int height = img.height();

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QColor col = img.pixelColor(x, y);
            float r = col.red() / 255.0f;
            float g = col.green() / 255.0f;
            float b = col.blue() / 255.0f;
            res.set(x, y, FLIP::color3(r, g, b));
        }
    }
    return res;
}

static FLIP::image<FLIP::color3> qImgToFlipImg(const QImage &img) {
    int width = img.width();
    int height = img.height();

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        const uchar *row = img.constScanLine(y);
        for (int x = 0; x < width; x++) {
            res.set(x, y, FLIP::color3(row[x] / 255.0f));
        }
    }
    return res;
}

static QImage flipImgToQimg(const FLIP::image<FLIP::color3> &img) {
    int width = img.getWidth();
    int height = img.getHeight();

    QImage res(width, height, QImage::Format_RGB32);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            FLIP::color3 col = img.get(x, y);
            int r = 255 * col.r + 0.5f;
            int g = 255 * col.g + 0.5f;
            int b = 255 * col.b + 0.5f;
            res.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return res;
}

inline static float calculatePPD(const float dist, const float resolutionX,
                                 const float monitorWidth) {
    return dist * (resolutionX / monitorWidth) * (float(FLIP::PI) / 180.0f);
}

QImage nitro::operations::flipCompare(const QImage &imageA, const QImage &imageB) {
    FLIP::image<FLIP::color3> fImgA = qColImgToFlipImg(imageA);
    FLIP::image<FLIP::color3> fImgB = qColImgToFlipImg(imageB);


    int width = imageA.width();
    int height = imageA.height();
    // TODO: extractt these constants
    int ppd = calculatePPD(0.7f, 1920, 0.7f);
    FLIP::image<float> errMap(width, height, 0.0f);
    errMap.FLIP(fImgA, fImgB, ppd);


    FLIP::image<FLIP::color3> colResult(width, height);
    colResult.copyFloat2Color3(errMap);
    FLIP::image<FLIP::color3> magmaMap(FLIP::MapMagma, 256);
    colResult.colorMap(errMap, magmaMap);
    return flipImgToQimg(colResult);
}
