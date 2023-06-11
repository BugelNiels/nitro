#include "imageviewalgorithm.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"

nitro::ImageViewAlgorithm::ImageViewAlgorithm(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool ok;
    auto img = nodePorts.getInputImage("Image", ok);
    if (ok) {
        imViewer_->setImage(*img);
        return;
    }
//    imViewer_->removeImage();
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImageViewAlgorithm::creator(const QString &category, ImageViewer* imageViewer) {
    return [category, imageViewer]() {
        nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.
                withOperator(std::make_unique<nitro::ImageViewAlgorithm>(imageViewer))->
                withInputImage("Image")->
                withIcon(":/icons/nodes/viewer.png")->
                withNodeColor({60, 29, 38})->
                build();
    };
}
