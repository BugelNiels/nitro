#include "surfaceviewoperator.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"

#define INPUT_IMAGE "Image"

nitro::SurfaceViewOperator::SurfaceViewOperator(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {
}

void nitro::SurfaceViewOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
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
                withInputPort<ColImageData>(INPUT_IMAGE)->
                build();
    };
}
