#pragma once

#include <QMainWindow>
#include "mainwindow.hpp"
#include "ui/surfacevis/renderview.hpp"

namespace nitro {

    class ImprocBuilder {

    public:
        ImprocBuilder();

        MainWindow *build();

        QWidget *initNodeTitleBar(MainWindow *window);

        QDockWidget *initSurfaceVisDock(MainWindow *window, nitro::RenderView *&surfView) const;

        NodeDockWidget *initNodeDock(MainWindow *window, ImageViewer *imView, RenderView *surfView);

        QDockWidget *initViewDock(nitro::MainWindow *window, nitro::ImageViewer *&imView);
    };

} // nitro
