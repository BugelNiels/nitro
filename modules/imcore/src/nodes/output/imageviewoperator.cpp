#include "imageviewoperator.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";

ImageViewOperator::ImageViewOperator(ImageViewer *imViewer)
        : imViewer_(imViewer) {
}


void ImageViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        imViewer_->removeImage();
        return;
    }
    auto im = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

    if (im->rows == 1 && im->cols == 1) {
        cv::resize(*im, *im, {256, 256});
    }
    imViewer_->setImage(im);

}

std::function<std::unique_ptr<NitroNode>()>
ImageViewOperator::creator(const QString &category, ImageViewer *imageViewer) {
    return [category, imageViewer]() {
        NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.
                withOperator(std::make_unique<ImageViewOperator>(imageViewer))->
                withIcon("viewer.png")->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
