#include "combinergbdatamodel.hpp"

#include "operations/conversions/rgbconvert.hpp"

nitro::CombineRgbDataModel::CombineRgbDataModel() : ColorCombineDataModel(3) {

}

void nitro::CombineRgbDataModel::combine(const QVector<std::shared_ptr<ImageData>> &input) {
    QImage red = input[0]->getDisplayImg();
    QImage green = input[1]->getDisplayImg();
    QImage blue = input[2]->getDisplayImg();

    auto colorImg = nitro::operations::combineRgb(red, green, blue);
    _result = std::make_shared<ImageData>(std::make_shared<QImage>(colorImg));
}
