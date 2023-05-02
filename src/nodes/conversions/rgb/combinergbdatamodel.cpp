#include "combinergbdatamodel.hpp"

nitro::CombineRgbDataModel::CombineRgbDataModel() : ColorCombineDataModel(3) {

}

void nitro::CombineRgbDataModel::combine(const QVector<std::shared_ptr<ImageData>> &input) {
    QImage red = input[0]->getDisplayImg();
    QImage green = input[1]->getDisplayImg();
    QImage blue = input[2]->getDisplayImg();


    int width = red.width();
    int height = red.height();
    QImage colorImg(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        uchar *redRow = red.scanLine(y);
        uchar *greenRow = green.scanLine(y);
        uchar *blueRow = blue.scanLine(y);
        for (int x = 0; x < width; x++) {
            colorImg.setPixelColor(x, y, QColor(redRow[x], greenRow[x], blueRow[x]));
        }
    }
    _result = std::make_shared<ImageData>(std::make_shared<QImage>(colorImg));

}
