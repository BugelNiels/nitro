#pragma once

#include <QMainWindow>
#include "mainwindow.hpp"

namespace nitro {

    class ImprocBuilder {

    public:
        ImprocBuilder();

        MainWindow *build();

        QWidget *initNodeTitleBar(MainWindow *window);

        QDockWidget *initSurfaceVisDock(MainWindow *window) const;

        NodeDockWidget *initNodeDock(MainWindow *window, ImageViewer *imView);

        QDockWidget *initViewDock(nitro::MainWindow *window, nitro::ImageViewer *&imView);
    };

} // nitro
