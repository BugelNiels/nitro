#include "combineictcpdatamodel.hpp"
#include "core/algorithms/conversions/ictcpconvert.hpp"


nitro::CombineICtCpDataModel::CombineICtCpDataModel() : ColorCombineDataModel(3) {

}

void nitro::CombineICtCpDataModel::combine(const QVector <std::shared_ptr<ImageData>> &input) {
    QImage iChannel = input[0]->getDisplayImg();
    QImage ctChannel = input[1]->getDisplayImg();
    QImage cpChannel = input[2]->getDisplayImg();

    auto colorImg = nitro::operations::combineICtCp(iChannel, ctChannel, cpChannel);
    _result = std::make_shared<ImageData>(std::make_shared<QImage>(colorImg));
}
