#include "improcbuilder.hpp"
#include "ui/imgviewer/imgviewer.hpp"
#include "components/zoombar.hpp"
#include "ui/surfacevis/renderview.hpp"
#include "datamodels/output/surfaceviewerdatamodel.hpp"
#include "config.hpp"
#include <QDockWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSplitter>
#include "src/core/nodes/initialization/nitronodes.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/DataColors.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/internal/WidgetNodePainter.hpp"

nitro::ImprocBuilder::ImprocBuilder() {}

nitro::MainWindow *nitro::ImprocBuilder::build() {
    auto *window = new nitro::MainWindow();

// Image viewer
    nitro::ImageViewer *imView;
    auto *imViewDock = initViewDock(window, imView);


    // Surface visualizer
    nitro::RenderView *surfView;
    QDockWidget *surfViewDock = initSurfaceVisDock(window, surfView);

    // Node editor

    nitro::NodeDockWidget *nodeDock = initNodeDock(window, imView, surfView);

    // Image viewer, surface visualizer split
    auto *horLayout = new QSplitter(Qt::Horizontal, window);
    horLayout->addWidget(imViewDock);
    horLayout->addWidget(surfViewDock);
    horLayout->setStretchFactor(0, 1);
    horLayout->setStretchFactor(1, 1);

    // Node editor, visualizers split
    auto *vertLayout = new QSplitter(Qt::Vertical, window);
    vertLayout->addWidget(horLayout);
    vertLayout->addWidget(nodeDock);
    vertLayout->setStretchFactor(0, 1);
    vertLayout->setStretchFactor(1, 1);

    horLayout->setSizes({window->width() / 2, window->width() / 2}); // Temp fix for equal sizes


    window->setCentralWidget(vertLayout);


    window->finalizeSetup();
    return window;
}

QDockWidget *
nitro::ImprocBuilder::initViewDock(nitro::MainWindow *window,
                                   nitro::ImageViewer *&imView) {
    auto *imViewDock = new QDockWidget("Image Viewer", window);
    imView = new ImageViewer(new QGraphicsScene(), imViewDock);

    auto *imViewTitleWrapper = new QWidget();
    auto *imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/image_viewer.png"));

    imHLayout->addStretch();

    auto *zoomLabel = new QLabel("zoom:");
    imHLayout->addWidget(zoomLabel);

    auto *zoomBar = new ZoomBar(imView->minScaleFactor * 100.0, imView->maxScaleFactor * 100.0);
    QPalette palette = zoomBar->palette();
    palette.setColor(QPalette::Highlight, QColor(60, 60, 60)); // set the color to red
    zoomBar->setPalette(palette);
    zoomBar->setMaximumWidth(200);
    imHLayout->addWidget(zoomBar);
    QObject::connect(imView, &nitro::ImageViewer::scaleChanged, window, [=](double scale) {
        zoomBar->setZoom(scale);
    });


    imViewTitleWrapper->setLayout(imHLayout);
    imViewDock->setTitleBarWidget(imViewTitleWrapper);
    imViewDock->setWidget(imView);
    imViewDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    window->registerDock(imViewDock);
    imViewDock->setFeatures(
            imViewDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    return imViewDock;
}

nitro::NodeDockWidget *nitro::ImprocBuilder::initNodeDock(MainWindow *window, ImageViewer *imView, RenderView *surfView) {
    // TODO: reduce pointer usage
    auto *nodes = new NitroNodes(imView, surfView);
    auto *dataFlowGraphModel = new QtNodes::DataFlowGraphModel(nodes->getRegistry());
    auto *nodeScene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);
    // TODO: better painter enabling
    nodeScene->setNodePainter(std::make_unique<QtNodes::WidgetNodePainter>(QtNodes::WidgetNodePainter()));
    nodeScene->toggleWidgetMode();
    auto *ngView = new ImageNodeGraphicsView(nodes, imView, nodeScene, dataFlowGraphModel, window);
    auto *nodeDock = new NodeDockWidget(ngView, window);
    nodeDock->setTitleBarWidget(initNodeTitleBar(window));
    nodeDock->setFeatures(nodeDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerNodeDock(nodeDock);
    return nodeDock;
}

QDockWidget *nitro::ImprocBuilder::initSurfaceVisDock(MainWindow *window, nitro::RenderView *&surfView) const {
    auto *surfViewDock = new QDockWidget("Surface Visualizer", window);
    surfViewDock->setTitleBarWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    surfView = new RenderView();

    surfViewDock->setWidget(surfView);

    // Surface view title TODO: clean
    auto *surfViewTitleWrapper = new QWidget();
    auto *surfHLayout = new QHBoxLayout();
    surfHLayout->addWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    surfHLayout->addStretch();

    auto *surfPerspectiveCheckBox = new QCheckBox("Orthographic");
    QObject::connect(surfPerspectiveCheckBox, &QCheckBox::toggled, window, [surfView] {
        surfView->toggleOrthographic();
    });
    surfHLayout->addWidget(surfPerspectiveCheckBox);

    auto *imageColorsCheckBox = new QCheckBox("Image Colors");
    imageColorsCheckBox->setChecked(true); // TODO: don't hardcode
    QObject::connect(imageColorsCheckBox, &QCheckBox::toggled, window, [surfView] {
        surfView->toggleImageColors();
    });
    surfHLayout->addWidget(imageColorsCheckBox);


    surfViewTitleWrapper->setLayout(surfHLayout);
    surfViewDock->setTitleBarWidget(surfViewTitleWrapper);
    surfViewDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    surfViewDock->setFeatures(
            surfViewDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerDock(surfViewDock);
    // TODO: do this elsewhere?
    SurfaceViewerDataModel::setSurfaceViewer(surfView);
    return surfViewDock;
}

QWidget *nitro::ImprocBuilder::initNodeTitleBar(nitro::MainWindow *window) {
    auto *wrapper = new QWidget();
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(window->buildDockIcon(":/icons/node_editor.png"));

    //  TODO: extract
    auto *nodeImgCheckBox = new QCheckBox("Node Images");
    nodeImgCheckBox->setChecked(nitro::config::nodeImages);
    nitro::MainWindow::connect(nodeImgCheckBox, &QCheckBox::toggled, window, [window, nodeImgCheckBox] {
        nitro::config::setNodeImages(nodeImgCheckBox->isChecked());
        window->getNodeDock()->recalculateNodeSizes();

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
