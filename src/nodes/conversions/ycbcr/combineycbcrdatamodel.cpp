#include "combineycbcrdatamodel.hpp"

#include "operations/conversions/ycbcrconvert.hpp"

nitro::CombineYCbrCrDataModel::CombineYCbrCrDataModel() : ColorCombineDataModel(3) {

}

void nitro::CombineYCbrCrDataModel::combine(const QVector<std::shared_ptr<ImageData>> &input) {
    QImage yChannel = input[0]->getDisplayImg();
    QImage cbChannel = input[1]->getDisplayImg();
    QImage crChannel = input[2]->getDisplayImg();

    auto colorImg = nitro::operations::combineYCbCr(yChannel, cbChannel, crChannel);
    _result = std::make_shared<ImageData>(std::make_shared<QImage>(colorImg));
}
