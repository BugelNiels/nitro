#include "mainwindow.hpp"
#include "nodeeditor/nodedockwidget.hpp"
#include "imgviewer/imgviewer.hpp"
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>

#include <QIcon>
#include <QKeyEvent>
#include <QEvent>
#include "util/imgconvert.hpp"
#include "util/imgresourcereader.hpp"

nitro::MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");

    setMenuBar(initMenuBar());
    setStatusBar(initFooter());

    const int icSize = 24;

    // Image viewer
    auto *imDock = new QDockWidget(this);
    auto *imIcon = new QLabel();
    imIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/image_viewer.png", {icSize, icSize}));
    imDock->setTitleBarWidget(imIcon);
    auto *imView = new nitro::ImageViewer(new QGraphicsScene(), imDock);
    imDock->setWidget(imView);

    // Surface visualizer
    auto *surfDock = new QDockWidget("Surface Visualizer", this);
    auto *surfIcon = new QLabel();
    surfIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/surface_visualizer.png", {icSize, icSize}));
    surfDock->setTitleBarWidget(surfIcon);

    // Node editor
    nodeDock = new nitro::NodeDockWidget(imView, this);
    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/node_editor.png", {icSize, icSize}));
    nodeDock->setTitleBarWidget(nodeIcon);

    // Image viewer, surface visualizer split
    auto *horLayout = new QSplitter(Qt::Horizontal, this);
    horLayout->addWidget(imDock);
    horLayout->addWidget(surfDock);
    horLayout->setStretchFactor(0, 1);
    horLayout->setStretchFactor(1, 1);

    // Node editor, visualizers split
    auto *vertLayout = new QSplitter(Qt::Vertical, this);
    vertLayout->addWidget(horLayout);
    vertLayout->addWidget(nodeDock);
    vertLayout->setStretchFactor(0, 1);
    vertLayout->setStretchFactor(1, 1);

    // Disable the undocking features
    imDock->setFeatures(imDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    surfDock->setFeatures(surfDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    nodeDock->setFeatures(nodeDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));

    setCentralWidget(vertLayout);
    installEventFilter(this);
}

QStatusBar *nitro::MainWindow::initFooter() {
    auto *versionLabel = new QLabel(" version 0.1 ", this);
    fileNameLabel = new QLabel(" untitled.json ", this);

    auto *statusBar = new QStatusBar(this);
    statusBar->insertPermanentWidget(0, fileNameLabel, 1);
    statusBar->insertPermanentWidget(1, versionLabel, 0);
    statusBar->setSizeGripEnabled(false);
    statusBar->setStyleSheet("color: grey;");
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
                if (nodeDock) {
                    if (!nodeDock->canQuitSafely()) {
                        return;
                    }
                    fileNameLabel->setText(nodeDock->getFileName());
                    nodeDock->clearModel();
                }
            });
    fileMenu->addAction(newAction);

    auto *openAction = new QAction(QStringLiteral("Open"), this);
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction,
            &QAction::triggered,
            [this]() {
                if (nodeDock) {
                    nodeDock->loadModel();
                    fileNameLabel->setText(nodeDock->getFileName());
                }
            });
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();

    auto *saveAction = new QAction(QStringLiteral("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction,
            &QAction::triggered,
            [this]() {
                if (nodeDock) {
                    nodeDock->saveModel();
                    fileNameLabel->setText(nodeDock->getFileName());
                }
            });
    fileMenu->addAction(saveAction);

    auto *saveAsAction = new QAction(QStringLiteral("Save As..."), this);
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction,
            &QAction::triggered,
            [this]() {
                if (nodeDock) {
                    nodeDock->saveModel(true);
                    fileNameLabel->setText(nodeDock->getFileName());
                }
            });
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();

    auto *quitAction = new QAction(QStringLiteral("Quit"), this);
    quitAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
    quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(quitAction,
            &QAction::triggered,
            [this]() {
                if (nodeDock) {
                    if (!nodeDock->canQuitSafely()) {
                        return;
                    }
                }
                exit(EXIT_SUCCESS);
            });
    fileMenu->addAction(quitAction);

    menuBar->addMenu(fileMenu);

    // TODO: save dock state somewhere and restore that here
    auto *windowMenu = new QMenu("Window");
    windowMenu->addAction("Node Editor");
    windowMenu->addAction("Image Viewer");
    windowMenu->addAction("Surface Visualizer");
    menuBar->addMenu(windowMenu);
    menuBar->setStyleSheet("QMenuBar { background-color: rgb(28, 28, 28); }");
    return menuBar;
}
