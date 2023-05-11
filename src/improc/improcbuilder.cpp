#include "improcbuilder.hpp"
#include "ui/imgviewer/imgviewer.hpp"
#include "util/imgresourcereader.hpp"
#include "components/zoombar.hpp"
#include "ui/surfacevis/renderview.hpp"
#include "datamodels/output/surfaceviewerdatamodel.hpp"
#include "config.hpp"
#include "datamodels/input/imagesourcedatamodel.hpp"
#include <QDockWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSplitter>
#include "datamodels/nitronodes.hpp"
#include "src/gui/nodeeditor/style/nodecolors.hpp"
#include "nodeeditor/style/datacolors.hpp"
#include "datamodels/datatypes/decimaldata.hpp"

nitro::ImprocBuilder::ImprocBuilder() {}

nitro::MainWindow *nitro::ImprocBuilder::build() {
    nitro::MainWindow *window = new nitro::MainWindow();

// Image viewer
    nitro::ImageViewer *imView;
    auto *imViewDock = initViewDock(window, imView);


    // Surface visualizer
    QDockWidget *surfViewDock = initSurfaceVisDock(window);

    // Node editor

    nitro::NodeDockWidget *nodeDock = initNodeDock(window, imView);


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

nitro::NodeDockWidget *nitro::ImprocBuilder::initNodeDock(nitro::MainWindow *window, nitro::ImageViewer *imView) {
    registerColors();
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    auto *dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    auto *nodeScene = new NodeGraphicsScene(*dataFlowGraphModel);
    auto *ngView = new ImageNodeGraphicsView(imView, nodeScene, dataFlowGraphModel, window);
    auto *nodeDock = new NodeDockWidget(ngView, window);
    nodeDock->setTitleBarWidget(initNodeTitleBar(window));
    nodeDock->setFeatures(nodeDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerNodeDock(nodeDock);
    return nodeDock;
}

QDockWidget *nitro::ImprocBuilder::initSurfaceVisDock(nitro::MainWindow *window) const {
    QDockWidget *surfViewDock = new QDockWidget("Surface Visualizer", window);
    surfViewDock->setTitleBarWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    auto *_renderView = new RenderView();
    surfViewDock->setWidget(_renderView);

    // Surface view title TODO: clean
    auto *surfViewTitleWrapper = new QWidget();
    auto *surfHLayout = new QHBoxLayout();
    surfHLayout->addWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    surfHLayout->addStretch();

    auto *surfPerspectiveCheckBox = new QCheckBox("Orthographic");
    QObject::connect(surfPerspectiveCheckBox, &QCheckBox::toggled, window, [_renderView] {
        _renderView->toggleOrthographic();
    });
    surfHLayout->addWidget(surfPerspectiveCheckBox);

    auto *imageColorsCheckBox = new QCheckBox("Image Colors");
    imageColorsCheckBox->setChecked(true); // TODO: don't hardcode
    QObject::connect(imageColorsCheckBox, &QCheckBox::toggled, window, [_renderView] {
        _renderView->toggleImageColors();
    });
    surfHLayout->addWidget(imageColorsCheckBox);


    surfViewTitleWrapper->setLayout(surfHLayout);
    surfViewDock->setTitleBarWidget(surfViewTitleWrapper);
    surfViewDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    surfViewDock->setFeatures(
            surfViewDock->features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerDock(surfViewDock);
    // TODO: do this elsewhere?
    SurfaceViewerDataModel::setSurfaceViewer(_renderView);
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

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> nitro::ImprocBuilder::registerDataModels() {
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    // TODO: better categories
    ret->registerModel<nitro::ImageSourceDataModel>("Input");
    ret->registerModel<nitro::IntegerSourceDataModel>("Input");
    ret->registerModel<nitro::DecimalSourceDataModel>("Input");
    ret->registerModel<nitro::ImageViewerDataModel>("Output");
    ret->registerModel<nitro::SurfaceViewerDataModel>("Output");
    ret->registerModel<nitro::ToGrayScaleDataModel>("Converter");
    ret->registerModel<nitro::ThresholdDataModel>("Operator");
    ret->registerModel<nitro::KMeansDataModel>("Operator");
    ret->registerModel<nitro::QuantisizeDataModel>("Operator");
    ret->registerModel<nitro::FlipDataModel>("Operator");
    ret->registerModel<nitro::ResampleDataModel>("Operator");
    ret->registerModel<nitro::SeparateRgbDataModel>("Operator");
    ret->registerModel<nitro::CombineRgbDataModel>("Operator");
    ret->registerModel<nitro::SeparateYCbCrDataModel>("Operator");
    ret->registerModel<nitro::CombineYCbrCrDataModel>("Operator");
    ret->registerModel<nitro::SeparateICtCpDataModel>("Operator");
    ret->registerModel<nitro::CombineICtCpDataModel>("Operator");
    ret->registerModel<nitro::ImgMathDataModel>("Operator");
    ret->registerModel<nitro::ToggleDataModel>("Operator");
    ret->registerModel<nitro::BlendDataModel>("Operator");
    ret->registerModel<nitro::SlrDataModel>("Operator");
    ret->registerModel<nitro::LayerSelectionDataModel>("Operator");
    ret->registerModel<nitro::LowPassFilterDataModel>("Operator");
    ret->registerModel<nitro::MixDataModel>("Operator");
    return ret;
}

void nitro::ImprocBuilder::registerColors() {
    nitro::NodeColors::registerColor(nitro::ImageSourceDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::IntegerSourceDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::DecimalSourceDataModel::nodeInfo());

    nitro::NodeColors::registerColor(nitro::ImageViewerDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::SurfaceViewerDataModel::nodeInfo());

    nitro::NodeColors::registerColor(nitro::ToGrayScaleDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::ThresholdDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::KMeansDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::QuantisizeDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::LayerSelectionDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::SlrDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::FlipDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::ResampleDataModel::nodeInfo());

    nitro::NodeColors::registerColor(nitro::CombineRgbDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::CombineYCbrCrDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::SeparateYCbCrDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::SeparateRgbDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::ImgMathDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::ToggleDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::BlendDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::LowPassFilterDataModel::nodeInfo());
    nitro::NodeColors::registerColor(nitro::MixDataModel::nodeInfo());

    // Data colors
    nitro::DataColors::registerColor(nitro::ImageData::dataInfo());
    nitro::DataColors::registerColor(nitro::IntegerData::dataInfo());
    nitro::DataColors::registerColor(nitro::DecimalData::dataInfo());
}

