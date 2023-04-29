#include "mainwindow.hpp"
#include "nodeview.hpp"
#include "imageview.hpp"
#include "src/gui/imgviewer/imgviewer.hpp"
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>

#include "util/imgconvert.hpp"

nitro::MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");

    setMenuBar(initMenuBar());
    setStatusBar(initFooter());

    // Image viewer
    auto *imDock = new QDockWidget("Image Viewer", this);

    auto *imScene = new ImageViewerScene();
    auto *imView = new nitro::ImageViewer(imScene, imDock);
    imDock->setWidget(imView);

    // Surface visualizer
    auto *surfDock = new QDockWidget("Surface Visualizer", this);

    // Node editor
    nodeView = new nitro::NodeView(imView, this);

    // Image viewer, surface visualizer split
    auto *horLayout = new QSplitter(Qt::Horizontal, this);
    horLayout->addWidget(imDock);
    horLayout->addWidget(surfDock);
    horLayout->setStretchFactor(0, 1);
    horLayout->setStretchFactor(1, 1);

    // Node editor, visualizers split
    auto *vertLayout = new QSplitter(Qt::Vertical, this);
    vertLayout->addWidget(horLayout);
    vertLayout->addWidget(nodeView);
    vertLayout->setStretchFactor(0, 1);
    vertLayout->setStretchFactor(1, 1);

    // Disable the undocking features
    imDock->setFeatures(imDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    surfDock->setFeatures(surfDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    nodeView->setFeatures(nodeView->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));

    setCentralWidget(vertLayout);
    centralWidget()->setFocus();
}

QStatusBar *nitro::MainWindow::initFooter() {
    auto *footerLabel = new QLabel("version 0.1", this);

    auto *statusBar = new QStatusBar(this);
    statusBar->insertPermanentWidget(0, footerLabel, 0);
    statusBar->setSizeGripEnabled(false);
    return statusBar;
}

nitro::MainWindow::~MainWindow() {}

QMenuBar *nitro::MainWindow::initMenuBar() {
    auto *menuBar = new QMenuBar();

    auto *fileMenu = new QMenu("File");

    auto *newAction = new QAction(QStringLiteral("New"), this);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newAction,
            &QAction::triggered,
            [this]() {
                if (nodeView) {
                    nodeView->clearModel();
                }
            });
    fileMenu->addAction(newAction);

    auto *openAction = new QAction(QStringLiteral("Open"), this);
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction,
            &QAction::triggered,
            []() {
                qDebug() << "Opening...";
                // TODO: open scene
            });
    fileMenu->addAction(openAction);

    auto *saveAction = new QAction(QStringLiteral("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction,
            &QAction::triggered,
            []() {
                qDebug() << "Saving...";
                // TODO: save scene
            });
    fileMenu->addAction(saveAction);

    auto *quitAction = new QAction(QStringLiteral("Quit"), this);
    quitAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
    quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(quitAction,
            &QAction::triggered,
            []() {
                // TODO: check save
                exit(EXIT_SUCCESS);
            });
    fileMenu->addAction(quitAction);

    menuBar->addMenu(fileMenu);

    // TODO: save dock state somewhere and restore that here
    // Actions should start disabled
    auto *windowMenu = new QMenu("Window");
    windowMenu->addAction("Node Editor");
    windowMenu->addAction("Image Viewer");
    windowMenu->addAction("Surface Visualizer");
    menuBar->addMenu(windowMenu);

    return menuBar;
}
