#include "imageviewalgorithm.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"

nitro::ImageViewAlgorithm::ImageViewAlgorithm(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    QString paramName = "image";
    bool ok;
    auto img = nodePorts.getInputImage("image", ok);
    if (ok) {
        imViewer_->setImage(toQImage(*img));
        return;
    }
//    imViewer_->removeImage();
}
