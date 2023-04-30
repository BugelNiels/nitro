#include <omp.h>

#include <QApplication>
#include <QSurfaceFormat>

#include "gui/mainwindow.hpp"
#include "util/imgconvert.hpp"
#include <QStyle>

QPalette getDarkModePalette() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(30, 30, 30));             // Window background color
    palette.setColor(QPalette::WindowText, QColor(239, 239, 239));      // Text color
    palette.setColor(QPalette::Base, QColor(40, 40, 40));               // Base color
    palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));      // Alternate base color
    palette.setColor(QPalette::ToolTipBase, QColor(239, 239, 239));     // ToolTip base color
    palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));           // ToolTip text color
    palette.setColor(QPalette::Text, QColor(239, 239, 239));            // Text color
    palette.setColor(QPalette::Button, QColor(70, 70, 70));             // Button color
    palette.setColor(QPalette::ButtonText, QColor(239, 239, 239));      // Button text color
    palette.setColor(QPalette::Highlight, QColor(75, 110, 175));        // Highlight color
    palette.setColor(QPalette::HighlightedText, QColor(239, 239, 239)); // Highlight text color

    return palette;
}

/**
 * @brief main Starts up the QT application and UI.
 * @param argc Argument count.
 * @param argv Arguments.
 * @return Exit code.
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
    omp_set_num_threads(8);

    nitro::MainWindow w;
    w.show();

    QPalette palette = getDarkModePalette();
    QApplication::setPalette(palette);

    return QApplication::exec();
}
