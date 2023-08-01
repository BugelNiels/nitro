#include "surfaceviewoperator.hpp"
#include "gui/im3dviewer/renderdockwidget.hpp"
#include <colimagedata.hpp>
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>

namespace nitro::Thesis {

static inline const QString INPUT_IMAGE = "Image";

SurfaceViewOperator::SurfaceViewOperator(MainWindow *window) : window_(window) {
    if (window->isFinalized()) {
        // TODO:
        surfViewer_ = new RenderView();
        dockWidget_ = new RenderDockWidget(surfViewer_, window);
        window->registerDock(dockWidget_);
    }
}

SurfaceViewOperator::~SurfaceViewOperator() {
    if (window_->isFinalized()) {
        window_->removeDockWidget(dockWidget_);
        delete surfViewer_;
        delete dockWidget_;
    }
}

void SurfaceViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    surfViewer_->updateBuffers(cvMatToQImage(*img, currentImg_));
}

std::function<std::unique_ptr<NitroNode>()> SurfaceViewOperator::creator(const QString &category,
                                                                         MainWindow *window) {
    return [category, window]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
        return builder.withOperator(std::make_unique<SurfaceViewOperator>(window))
                ->withIcon("3d.png")
                ->withNodeColor(NITRO_OUTPUT_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::Thesis
