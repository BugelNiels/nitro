#include "imageviewoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"

#define INPUT_IMAGE "Image"

#include <QDebug>

nitro::ImageViewAlgorithm::ImageViewAlgorithm(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        imViewer_->removeImage();
        return;
    }
    auto im = ImageData::from(nodePorts.inGet(INPUT_IMAGE));

    if(im->rows == 1 && im->cols == 1) {
        cv::resize(*im, *im, {256, 256});
    }
    imViewer_->setImage(im);

}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageViewAlgorithm::creator(const QString &category, ImageViewer *imageViewer) {
    return [category, imageViewer]() {
        nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.
                withOperator(std::make_unique<nitro::ImageViewAlgorithm>(imageViewer))->
                withIcon("viewer.png")->
                withNodeColor({60, 29, 38})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                build();
    };
}
