#include "seperatergbdatamodel.hpp"

nitro::SeperateRgbDataModel::SeperateRgbDataModel() : ColorSeparateDataModel(3) {

}

void nitro::SeperateRgbDataModel::separate(const QImage &img) {
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
    _result[0] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(red));
    _result[1] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(green));
    _result[2] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(blue));
}
