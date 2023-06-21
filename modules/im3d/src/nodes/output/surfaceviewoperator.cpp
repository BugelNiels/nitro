#include "surfaceviewoperator.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"

#define INPUT_IMAGE "Image"

nitro::SurfaceViewOperator::SurfaceViewOperator(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {
}

void nitro::SurfaceViewOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    surfViewer_->updateBuffers(cvMatToQImage(*img, currentImg_));
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::SurfaceViewOperator::creator(const QString &category, RenderView *renderViewer) {
    return [category, renderViewer]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
        return builder.
                withOperator(std::make_unique<nitro::SurfaceViewOperator>(renderViewer))->
                withIcon("3d.png")->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<ImageData>(INPUT_IMAGE)->
                build();
    };
}
