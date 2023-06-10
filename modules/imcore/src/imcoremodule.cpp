#include "imcoremodule.hpp"

#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"

#include "nodes/datatypes/decimaldata.hpp"
#include "gui/imviewer/imviewer.hpp"
#include "gui/imviewer/imviewdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

namespace nitro::ImCore {

    ImCoreModule::ImCoreModule() {
        imageViewer_ = new ImageViewer(new QGraphicsScene());
    }

    void ImCoreModule::registerNodes(NodeRegistry *registry) {
        registerInputNodes(registry);
        registerOutputNodes(registry, imageViewer_);
        registerUtilNodes(registry);
        registerImageNodes(registry);
    }

    void ImCoreModule::registerDataTypes(NodeRegistry *registry) {
        registry->registerDataType(ImageData::dataInfo());
        registry->registerDataType(IntegerData::dataInfo());
        registry->registerDataType(DecimalData::dataInfo());
    }

    void ImCoreModule::registerDocks(nitro::MainWindow *window) {
        auto imViewDock = new ImViewDockWidget(imageViewer_, window);
        window->registerDock(imViewDock);
    }

} // imCore