#include "surfaceviewalgorithm.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"


nitro::SurfaceViewAlgorithm::SurfaceViewAlgorithm(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {

}

void nitro::SurfaceViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool ok;
    auto img = nodePorts.getInputImage("image", ok);
    if (ok) {
        surfViewer_->updateBuffers(cvMatToQImage(*img));
        return;
    }
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::SurfaceViewAlgorithm::creator(const QString &category, RenderView *renderViewer) {
    return [category, renderViewer]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
        return builder.
                withOperator(std::make_unique<nitro::SurfaceViewAlgorithm>(renderViewer))->
                withInputImage("image")->
                withIcon(":/icons/nodes/surface.png")->
                withNodeColor({99, 28, 28})->
                build();
    };
}
