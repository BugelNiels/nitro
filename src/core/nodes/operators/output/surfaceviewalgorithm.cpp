#include "surfaceviewalgorithm.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/imagedata.hpp"


nitro::SurfaceViewAlgorithm::SurfaceViewAlgorithm(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {

}


void nitro::SurfaceViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    QString paramName = "image";
    auto inputImgDatGrey = nodePorts.getInDataAsType<nitro::GreyImageData>(paramName);
    if (inputImgDatGrey != nullptr) {
        surfViewer_->updateBuffers(inputImgDatGrey->getDisplayImg());
        return;
    }
    auto inputImgDatCol = nodePorts.getInDataAsType<nitro::ColorImageData>(paramName);
    if (inputImgDatCol != nullptr) {
        surfViewer_->updateBuffers(inputImgDatCol->getDisplayImg());
        return;
    }
    auto inputImgDat = nodePorts.getInDataAsType<nitro::ImageData>(paramName);
    if (inputImgDatCol != nullptr) {
        surfViewer_->updateBuffers(inputImgDat->getDisplayImg());
        return;
    }
}
