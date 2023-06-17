#include "surfaceviewalgorithm.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"

#define INPUT_IMAGE "Image"

nitro::SurfaceViewAlgorithm::SurfaceViewAlgorithm(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {
    currentImg_ = std::make_shared<cv::Mat>();
}

void nitro::SurfaceViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (nodePorts.inputsPresent({INPUT_IMAGE})) {
        auto img = nodePorts.getInputImage(INPUT_IMAGE);
        img->copyTo(*currentImg_);
        surfViewer_->updateBuffers(cvMatToQImage(currentImg_));
        return;
    }
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::SurfaceViewAlgorithm::creator(const QString &category, RenderView *renderViewer) {
    return [category, renderViewer]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
        return builder.
                withOperator(std::make_unique<nitro::SurfaceViewAlgorithm>(renderViewer))->
                withIcon("surface.png")->
                withNodeColor({99, 28, 28})->
                withInputImage(INPUT_IMAGE)->
                build();
    };
}
