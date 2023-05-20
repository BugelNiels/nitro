#include "separateictcpdatamodel.hpp"
#include "core/algorithms/conversions/ictcpconvert.hpp"


nitro::SeparateICtCpDataModel::SeparateICtCpDataModel() : ColorSeparateDataModel(3) {

}

void nitro::SeparateICtCpDataModel::separate(const QImage &img) {
    auto channels = nitro::operations::separateICtCp(img);
    for (int i = 0; i < 3; i++) {
        _result[i] = std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(channels[i]));
    }
}
