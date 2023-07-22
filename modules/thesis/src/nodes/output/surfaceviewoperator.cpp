#include "surfaceviewoperator.hpp"
#include <colimagedata.hpp>
#include <util.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::Thesis {

static inline const QString INPUT_IMAGE = "Image";

SurfaceViewOperator::SurfaceViewOperator(RenderView *surfViewer)
        : surfViewer_(surfViewer) {
}

void SurfaceViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    surfViewer_->updateBuffers(cvMatToQImage(*img, currentImg_));
}

std::function<std::unique_ptr<NitroNode>()>
SurfaceViewOperator::creator(const QString &category, RenderView *renderViewer) {
    return [category, renderViewer]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
        return builder.
                withOperator(std::make_unique<SurfaceViewOperator>(renderViewer))->
                withIcon("3d.png")->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                build();
    };
}
} // namespace nitro::Thesis
