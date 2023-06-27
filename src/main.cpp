#if _OPENMP

#include <omp.h>

#endif

#include <QApplication>
#include <QSurfaceFormat>

#include "gui/mainwindow.hpp"
#include <QStyle>
#include <QFontDatabase>

#include "QtNodes/internal/WidgetNodePainter.hpp"
#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "config.hpp"

#include "src/gui/stylepresets.hpp"

void setupApplication() {
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
#ifdef _OPENMP
    omp_set_num_threads(8);
#else
    std::cerr << "Warning: OpenMP not found. No parallelization available." << std::endl;
#endif
    nitro::applyStylePreset(nitro::getDarkModePalette());

    QFont font("Lato");
    font.setPixelSize(14);
    QApplication::setFont(font);
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(NITRO_NAME);
    QApplication::setApplicationVersion(NITRO_VERSION);
    setupApplication();

    std::vector<std::unique_ptr<nitro::NitroModule>> modules = nitro::initModules();

    auto *nodes = new nitro::NodeRegistry();
    for (auto &nitroModule: modules) {
        nitroModule->registerDataTypes(nodes);
        nitroModule->registerNodes(nodes);
    }
    auto *window = new nitro::MainWindow(nodes);
    for (auto &nitroModule: modules) {
        nitroModule->registerDocks(window);
    }

    window->finalizeSetup();
    window->show();
    return QApplication::exec();
}
