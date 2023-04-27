#include <omp.h>

#include <QApplication>
#include <QSurfaceFormat>

#include "gui/mainwindow.hpp"
#include "util/imgconvert.hpp"

/**
 * @brief main Starts up the QT application and UI.
 * @param argc Argument count.
 * @param argv Arguments.
 * @return Exit code.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
    omp_set_num_threads(8);

    MainWindow w;
    w.show();

    return a.exec();
}
