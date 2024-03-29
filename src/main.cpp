#if _OPENMP
#include <omp.h>
#endif

#include <QApplication>
#include <QSurfaceFormat>

#include <QFontDatabase>
#include <QStyle>
#include <gui/mainwindow.hpp>

#include <QtNodes/internal/WidgetNodePainter.hpp>
#include <config.hpp>
#include <nitromodule.hpp>
#include <nodes/noderegistry.hpp>

#include "src/gui/stylepresets.hpp"

void setupApplication() {
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
#ifdef _OPENMP
    omp_set_num_threads(8);
    cv::setNumThreads(8);
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

    auto nodes = std::make_shared<nitro::NodeRegistry>();
    auto *window = new nitro::MainWindow();
    for (auto &nitroModule: modules) {
        nitroModule->registerDataTypes();
        nitroModule->registerNodes(nodes, window);
    }

    window->finalizeSetup(nodes);
    window->show();
    return QApplication::exec();
}
