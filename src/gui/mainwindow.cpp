#include "mainwindow.hpp"
#include "nodeeditor/nodedockwidget.hpp"
#include "imgviewer/imgviewer.hpp"
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>

#include <QIcon>
#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QProgressBar>
#include "util/imgresourcereader.hpp"
#include "config.hpp"
#include "ZoomBar.h"
#include "surfacevis/renderview.hpp"
#include "output/surfaceviewerdatamodel.hpp"

nitro::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("NITRO");



    // TODO: extract
    // Image viewer
    imViewDock = new QDockWidget(this);
    nitro::ImageViewer *imView;
    imView = new ImageViewer(new QGraphicsScene(), imViewDock);

    auto *imViewTitleWrapper = new QWidget();
    auto *imHLayout = new QHBoxLayout();

    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/image_viewer.png", {icSize, icSize}, icColor));
    imHLayout->addWidget(nodeIcon);

    imHLayout->addStretch();

    auto *zoomLabel = new QLabel("zoom:");
    imHLayout->addWidget(zoomLabel);

    auto *zoomBar = new nitro::ZoomBar(imView->minScaleFactor * 100.0, imView->maxScaleFactor * 100.0);
    QPalette palette = zoomBar->palette();
    palette.setColor(QPalette::Highlight, QColor(60, 60, 60)); // set the color to red
    zoomBar->setPalette(palette);
    zoomBar->setMaximumWidth(200);
    imHLayout->addWidget(zoomBar);
    connect(imView, &nitro::ImageViewer::scaleChanged, this, [=](double scale) {
        zoomBar->setZoom(scale);
    });


    imViewTitleWrapper->setLayout(imHLayout);
    imViewDock->setTitleBarWidget(imViewTitleWrapper);
    imViewDock->setWidget(imView);

    // Surface visualizer
    surfViewDock = new QDockWidget("Surface Visualizer", this);
    auto *surfIcon = new QLabel();
    surfIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/surface_visualizer.png", {icSize, icSize}, icColor));
    surfIcon->setMargin(icMargin);
    surfViewDock->setTitleBarWidget(surfIcon);
    auto* _renderView = new nitro::RenderView();
    surfViewDock->setWidget(_renderView);

    // TODO: do this elsewhere?
    nitro::SurfaceViewerDataModel::setSurfaceViewer(_renderView);

    // Node editor
    nodeDock = new nitro::NodeDockWidget(imView, this);
    nodeDock->setTitleBarWidget(initNodeTitleBar());

    imViewDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    surfViewDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Image viewer, surface visualizer split
    auto *horLayout = new QSplitter(Qt::Horizontal, this);
    horLayout->addWidget(imViewDock);
    horLayout->addWidget(surfViewDock);
    horLayout->setStretchFactor(0, 1);
    horLayout->setStretchFactor(1, 1);

    // Node editor, visualizers split
    auto *vertLayout = new QSplitter(Qt::Vertical, this);
    vertLayout->addWidget(horLayout);
    vertLayout->addWidget(nodeDock);
    vertLayout->setStretchFactor(0, 1);
    vertLayout->setStretchFactor(1, 1);

    horLayout->setSizes({width() / 2, width() / 2}); // Temp fix for equal sizes

    // Disable the undocking features
    imViewDock->setFeatures(imViewDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    surfViewDock->setFeatures(surfViewDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    nodeDock->setFeatures(nodeDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));

    setCentralWidget(vertLayout);
    installEventFilter(this);


    setMenuBar(initMenuBar());
    setStatusBar(initFooter());
}


QStatusBar *nitro::MainWindow::initFooter() {
    // TODO: extract
    auto *versionLabel = new QLabel(" version 0.2 ", this);
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

    QMenu *fileMenu = getFileMenu();

    menuBar->addMenu(fileMenu);

    // TODO: save dock state somewhere and restore that here

    auto *editMenu = new QMenu("Edit");
    menuBar->addMenu(editMenu);
    auto *viewMenu = new QMenu("View");
    menuBar->addMenu(viewMenu);


    QMenu *windowMenu = getWindowMenu();
    menuBar->addMenu(windowMenu);
    menuBar->setStyleSheet("QMenuBar { background-color: rgb(28, 28, 28); }");
    return menuBar;
}

QMenu *nitro::MainWindow::getWindowMenu() {
    auto *windowMenu = new QMenu("Window");

    auto *nodeEditorAction = new QAction("Node Editor", this);
    nodeEditorAction->setCheckable(true);
    nodeEditorAction->setChecked(!nodeDock->isHidden());
    connect(nodeEditorAction, &QAction::triggered, [this]() {
        nodeDock->setHidden(!nodeDock->isHidden());
    });
    windowMenu->addAction(nodeEditorAction);

    auto *imageViewerAction = new QAction("Image Viewer", this);
    imageViewerAction->setCheckable(true);
    imageViewerAction->setChecked(!imViewDock->isHidden());
    connect(imageViewerAction, &QAction::triggered, [this]() {
        imViewDock->setHidden(!imViewDock->isHidden());
    });
    windowMenu->addAction(imageViewerAction);

    auto *surfViewerAction = new QAction("Surface Visualizer", this);
    surfViewerAction->setCheckable(true);
    surfViewerAction->setChecked(!surfViewDock->isHidden());
    connect(surfViewerAction, &QAction::triggered, [this]() {
        surfViewDock->setHidden(!surfViewDock->isHidden());
    });
    windowMenu->addAction(surfViewerAction);
    return windowMenu;
}

QMenu *nitro::MainWindow::getFileMenu() {
    auto *fileMenu = new QMenu("File");

    auto *newAction = new QAction(QStringLiteral("New"), this);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newAction, &QAction::triggered, [this]() {
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
    connect(openAction, &QAction::triggered, [this]() {
        if (nodeDock) {
            nodeDock->loadModel();
            fileNameLabel->setText(nodeDock->getFileName());
        }
    });
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();

    auto *saveAction = new QAction(QStringLiteral("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction, &QAction::triggered, [this]() {
        if (nodeDock) {
            nodeDock->saveModel();
            fileNameLabel->setText(nodeDock->getFileName());
        }
    });
    fileMenu->addAction(saveAction);

    auto *saveAsAction = new QAction(QStringLiteral("Save As..."), this);
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, [this]() {
        if (nodeDock) {
            nodeDock->saveModel(true);
            fileNameLabel->setText(nodeDock->getFileName());
        }
    });
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();

    auto *quitAction = new QAction(QStringLiteral("Quit"), this);
    quitAction->setShortcutContext(Qt::ApplicationShortcut);
    quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(quitAction, &QAction::triggered, [this]() {
        if (nodeDock) {
            if (!nodeDock->canQuitSafely()) {
                return;
            }
        }
        exit(EXIT_SUCCESS);
    });
    fileMenu->addAction(quitAction);
    return fileMenu;
}

QWidget *nitro::MainWindow::initNodeTitleBar() {
    auto *wrapper = new QWidget();
    auto *hLayout = new QHBoxLayout();

    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImgResourceReader::getPixMap(":/icons/node_editor.png", {icSize, icSize}, icColor));
    hLayout->addWidget(nodeIcon);

    auto *nodeImgCheckBox = new QCheckBox("Node Images");
    nodeImgCheckBox->setChecked(nitro::config::nodeImages);
    connect(nodeImgCheckBox, &QCheckBox::toggled, this, [this, nodeImgCheckBox] {
        nitro::config::setNodeImages(nodeImgCheckBox->isChecked());
        nodeDock->recalculateNodeSizes();

    });
    hLayout->addSpacing(15);
    hLayout->addWidget(nodeImgCheckBox);
    hLayout->addStretch();

    auto *calcProgressBar = new QProgressBar();
    calcProgressBar->setMinimum(0);
    calcProgressBar->setMaximum(100);
    calcProgressBar->setMaximumWidth(200);
    hLayout->addWidget(calcProgressBar);

    wrapper->setLayout(hLayout);
    return wrapper;
}
