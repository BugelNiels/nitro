#include "imcoremodule.hpp"

#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"

#include "nodes/datatypes/decimaldata.hpp"
#include "modules/imCore/gui/imgviewer/imviewdockwidget.hpp"
#include "modules/imCore/gui/surfacevis/renderdockwidget.hpp"
#include "modules/imCore/gui/surfacevis/renderview.hpp"
#include "modules/imCore/gui/imgviewer/imgviewer.hpp"

namespace nitro::imCore {

    ImCoreModule::ImCoreModule() {
        imageViewer_ = new ImageViewer(new QGraphicsScene());
        renderViewer_ = new RenderView();
    }

    void ImCoreModule::registerNodes(NodeRegistry *registry) {
        buildInputNodes(registry);
        buildOutputNodes(registry, imageViewer_, renderViewer_);
        buildConverterNodes(registry);
        buildComparisonNodes(registry);
        buildFilterNodes(registry);
        buildColorNodes(registry);
        buildQuantizationNodes(registry);
        buildResampleNodes(registry);
    }

    void ImCoreModule::registerDataTypes(NodeRegistry* registry) {
        registry->registerDataType(ImageData::dataInfo());
        registry->registerDataType(ColorImageData::dataInfo());
        registry->registerDataType(GreyImageData::dataInfo());
        registry->registerDataType(IntegerData::dataInfo());
        registry->registerDataType(DecimalData::dataInfo());
    }

    void ImCoreModule::registerDocks(nitro::MainWindow *window) {
        auto imViewDock = new ImViewDockWidget(imageViewer_, window);
        window->registerDock(imViewDock);

        auto im3DViewDock = new RenderDockWidget(renderViewer_, window);
        window->registerDock(im3DViewDock);
    }

} // imCore