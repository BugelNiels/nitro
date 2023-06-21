#include "imageviewoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"

#define INPUT_IMAGE "Image"

#include <QDebug>

nitro::ImageViewOperator::ImageViewOperator(nitro::ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void nitro::ImageViewOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        imViewer_->removeImage();
        return;
    }
    auto im = ColImageData::from(nodePorts.inGet(INPUT_IMAGE));

    if(im->rows == 1 && im->cols == 1) {
        cv::resize(*im, *im, {256, 256});
    }
    imViewer_->setImage(im);

}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageViewOperator::creator(const QString &category, ImageViewer *imageViewer) {
    return [category, imageViewer]() {
        nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.
                withOperator(std::make_unique<nitro::ImageViewOperator>(imageViewer))->
                withIcon("viewer.png")->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                build();
    };
}
