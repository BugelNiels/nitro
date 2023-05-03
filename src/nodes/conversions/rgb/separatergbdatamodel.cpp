#include "separatergbdatamodel.hpp"

#include "operations/conversions/rgbconvert.hpp"

nitro::SeparateRgbDataModel::SeparateRgbDataModel() : ColorSeparateDataModel(3) {

}

void nitro::SeparateRgbDataModel::separate(const QImage &img) {
    auto channels = nitro::operations::separateRgb(img);
    for (int i = 0; i < 3; i++) {
        _result[i] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(channels[i]));
    }
}
