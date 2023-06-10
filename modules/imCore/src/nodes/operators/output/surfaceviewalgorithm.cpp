#include "surfaceviewalgorithm.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"


nitro::SurfaceViewAlgorithm::SurfaceViewAlgorithm(nitro::RenderView *surfViewer)
        : surfViewer_(surfViewer) {

}

void nitro::SurfaceViewAlgorithm::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    QString paramName = "image";
    bool ok;
    auto img = nodePorts.getInputImage("image", ok);
    if (ok) {
        surfViewer_->updateBuffers(toQImage(*img));
        return;
    }
}
