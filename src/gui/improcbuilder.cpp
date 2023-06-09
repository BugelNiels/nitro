#include "improcbuilder.hpp"

#include <QHBoxLayout>
#include "external/nodeeditor/include/QtNodes/DataColors.hpp"
#include "external/nodeeditor/include/QtNodes/internal/WidgetNodePainter.hpp"
#include "modules/imCore/imcoremodule.hpp"

nitro::ImprocBuilder::ImprocBuilder() {}

nitro::MainWindow *nitro::ImprocBuilder::build() {

    std::vector<std::unique_ptr<NitroModule>> modules;
    modules.push_back(std::make_unique<imCore::ImCoreModule>());

    // Node editor
    auto *nodes = new NodeRegistry();

    for (auto &nitroModule: modules) {
        nitroModule->registerNodes(nodes);
        nitroModule->registerDataTypes(nodes);
    }
    auto *window = new MainWindow(nodes);
    for (auto &nitroModule: modules) {
        nitroModule->registerDocks(window);
    }

    window->finalizeSetup();
    return window;
}