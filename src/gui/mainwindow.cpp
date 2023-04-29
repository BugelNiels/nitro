#include "mainwindow.hpp"
#include "nodeview.hpp"
#include "imageview.hpp"
#include "src/gui/imgviewer/imgviewer.hpp"
#include <QFileDialog>
#include <QMessageBox>

#include "util/imgconvert.hpp"

nitro::MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");

    auto *imDock = new QDockWidget("Image Viewer", this);
    auto* imScene = new ImageViewerScene();
    auto *imView = new nitro::ImageViewer(imScene, imDock);
    imDock->setWidget(imView);
    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, imDock);


    auto *surfDock = new QDockWidget("Surface Visualizer", this);
    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, surfDock);

    setCentralWidget(new nitro::NodeView(imView, this));
    centralWidget()->setFocus();
    centralWidget()->setMinimumHeight(400);
}

nitro::MainWindow::~MainWindow() {}
