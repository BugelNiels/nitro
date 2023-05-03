#include "separateycbcrdatamodel.hpp"


#include "operations/conversions/ycbcrconvert.hpp"

nitro::SeparateYCbCrDataModel::SeparateYCbCrDataModel() : ColorSeparateDataModel(3) {

}

void nitro::SeparateYCbCrDataModel::separate(const QImage &img) {
    auto channels = nitro::operations::separateYCbCr(img);
    for (int i = 0; i < 3; i++) {
        _result[i] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(channels[i]));
    }
}
