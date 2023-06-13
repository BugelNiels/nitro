#include "imageviewalgorithm.hpp"
#include "nodes/nitronodebuilder.hpp"

#define INPUT_IMAGE "Image"

nitro::ImageViewAlgorithm::ImageViewAlgorithm(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (nodePorts.inputsPresent({INPUT_IMAGE})) {
        auto img = nodePorts.getInputImage(INPUT_IMAGE);
        imViewer_->setImage(*img);
        return;
    }
    imViewer_->removeImage();
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageViewAlgorithm::creator(const QString &category, ImageViewer *imageViewer) {
    return [category, imageViewer]() {
        nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.
                withOperator(std::make_unique<nitro::ImageViewAlgorithm>(imageViewer))->
                withIcon("viewer.png")->
                withNodeColor({60, 29, 38})->
                withInputImage(INPUT_IMAGE)->
                build();
    };
}
