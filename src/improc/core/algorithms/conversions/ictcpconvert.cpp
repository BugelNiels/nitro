
#include "ictcpconvert.hpp"

#include "util/util.hpp"
#include <QDebug>

static inline void iCtCp2Rgb(int i, int ct, int cp, uchar &r, uchar &g, uchar &b) {
    float fi = float(i) - 16.0f;
    float fct = float(ct) - 128.0f;
    float fcp = float(cp) - 128.0f;
    float fr = 1.569160f * fi + 0.184976f * fct + 0.332247f * fcp;
    float fg = 1.569160f * fi - 0.331665f * fct - 0.770735f * fcp;
    float fb = 1.569160f * fi + 0.544111f * fct - 0.214790f * fcp;
    r = nitro::clampConvert(fr);
    g = nitro::clampConvert(fg);
    b = nitro::clampConvert(fb);
}


static inline void rgb2ICtCp(int r, int g, int b, uchar &i, uchar &ct, uchar &cp) {
    float fr = float(r);
    float fg = float(g);
    float fb = float(b);
    float fi = 0.096621f * fr + 0.485232f * fg + 0.087337f * fb + 16.0f;
    float fct = -0.452217f * fr - 0.531045f * fg + 0.983262f * fb + 128.0f;
    float fcp = 0.617876f * fr - 0.204043f * fg - 0.413833f * fb + 128.0f;
    i = nitro::clampConvert(fi);
    ct = nitro::clampConvert(fct);
    cp = nitro::clampConvert(fcp);
}

QVector<QImage> nitro::operations::separateICtCp(const QImage &img) {
    int width = img.width();
    int height = img.height();
    // TODO: format for higher bit depths
    QImage iChannel(width, height, QImage::Format_Grayscale8);
    QImage ctChannel(width, height, QImage::Format_Grayscale8);
    QImage cpChannel(width, height, QImage::Format_Grayscale8);

    uchar i,ct,cp, r, g, b;
    rgb2ICtCp(0, 255, 255, i, ct, cp);
    qDebug() << i << ct << cp ;
    iCtCp2Rgb(i, ct, cp, r, g, b);
    qDebug() << r << g << b ;

    for (int y = 0; y < height; y++) {
        uchar *iRow = iChannel.scanLine(y);
        uchar *ctRow = ctChannel.scanLine(y);
        uchar *cpRow = cpChannel.scanLine(y);
        for (int x = 0; x < width; x++) {
            QColor col = img.pixelColor(x, y);
            rgb2ICtCp(col.red(), col.green(), col.blue(), iRow[x], ctRow[x],
                      cpRow[x]);
        }
    }
    return {iChannel, ctChannel, cpChannel};
}


QImage nitro::operations::combineICtCp(const QImage &iChannel, const QImage &ctChannel, const QImage &cpChannel) {

    int width = iChannel.width();
    int height = iChannel.height();
    QImage colorImg(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        const uchar *iRow = iChannel.constScanLine(y);
        const uchar *ctRow = ctChannel.constScanLine(y);
        const uchar *cpRow = cpChannel.constScanLine(y);
        for (int x = 0; x < width; x++) {
            uchar r, g, b;
            iCtCp2Rgb(iRow[x], ctRow[x], cpRow[x], r, g, b);
            colorImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return colorImg;
}