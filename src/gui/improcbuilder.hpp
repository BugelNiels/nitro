#pragma once

#include <QMainWindow>
#include "mainwindow.hpp"
#include "surfacevis/renderview.hpp"

namespace nitro {

    class ImprocBuilder {

    public:
        ImprocBuilder();

        MainWindow *build();

        QWidget *initNodeTitleBar(MainWindow *window);

        QDockWidget *initSurfaceVisDock(MainWindow *window, RenderView *&surfView) const;

        NodeDockWidget *initNodeDock(MainWindow *window, ImageViewer *imView, RenderView *surfView);

        QDockWidget *initViewDock(MainWindow *window, ImageViewer *&imView);
    };

} // nitro
