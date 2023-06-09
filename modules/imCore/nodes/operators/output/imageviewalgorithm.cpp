#include "imageviewalgorithm.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "modules/imCore/gui/imgviewer/imgviewer.hpp"

nitro::ImageViewAlgorithm::ImageViewAlgorithm(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    qDebug() << "Viewing image";
    QString paramName = "image";
    auto inputImgDatGrey = nodePorts.getInDataAsType<nitro::GreyImageData>(paramName);
    if (inputImgDatGrey != nullptr) {
        imViewer_->setImage(inputImgDatGrey->getDisplayImg());
        return;
    }
    auto inputImgDatCol = nodePorts.getInDataAsType<nitro::ColorImageData>(paramName);
    if (inputImgDatCol != nullptr) {
        imViewer_->setImage(inputImgDatCol->getDisplayImg());
        return;
    }
    auto inputImgDat = nodePorts.getInDataAsType<nitro::ImageData>(paramName);
    if (inputImgDatCol != nullptr) {
        imViewer_->setImage(inputImgDat->getDisplayImg());
        return;
    }
//    imViewer_->removeImage();
}
