#include <omp.h>

#include <QApplication>
#include <QSurfaceFormat>

#include "gui/mainwindow.hpp"
#include <QStyle>
#include <QFontDatabase>

#include "QtNodes/DataColors.hpp"
#include "QtNodes/internal/WidgetNodePainter.hpp"
#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "config.hpp"

void setupApplication();

QPalette getDarkModePalette() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(45, 45, 47));             // Window background color
    palette.setColor(QPalette::WindowText, QColor(239, 239, 239));      // Text color
    palette.setColor(QPalette::Base, QColor(40, 40, 40));               // Base color
    palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));      // Alternate base color
    palette.setColor(QPalette::ToolTipBase, QColor(239, 239, 239));     // ToolTip base color
    palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));           // ToolTip text color
    palette.setColor(QPalette::Text, QColor(239, 239, 239));            // Text color
    palette.setColor(QPalette::Button, QColor(70, 70, 72));             // Button color
    palette.setColor(QPalette::ButtonText, QColor(239, 239, 239));      // Button text color
    palette.setColor(QPalette::Highlight, QColor(75, 110, 175));        // Highlight color
    palette.setColor(QPalette::HighlightedText, QColor(239, 239, 239)); // Highlight text color
    palette.setColor(QPalette::PlaceholderText, QColor(150, 150, 150)); // PlaceHolder text color

    return palette;
}

void setupApplication() {
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
    omp_set_num_threads(8);

    QPalette palette = getDarkModePalette();
    QApplication::setPalette(palette);

    QFont font("Lato");
    font.setPixelSize(14);
    QApplication::setFont(font);
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    setupApplication();


    std::vector<std::unique_ptr<nitro::NitroModule>> modules = nitro::initModules();

    auto *nodes = new nitro::NodeRegistry();
    for (auto &nitroModule: modules) {
        nitroModule->registerNodes(nodes);
        nitroModule->registerDataTypes(nodes);
    }
    auto *window = new nitro::MainWindow(nodes);
    for (auto &nitroModule: modules) {
        nitroModule->registerDocks(window);
    }

    window->finalizeSetup();
    window->show();
    return QApplication::exec();
}
