#include "renderdockwidget.hpp"

#include <QHBoxLayout>
#include <QCheckBox>
#include "renderview.hpp"
#include "gui/mainwindow.hpp"

#define EASTER_EGG_MODE

nitro::RenderDockWidget::RenderDockWidget(RenderView *renderView, MainWindow *window)
        : QDockWidget(window),
          renderView_(renderView) {
    setWindowTitle("3D Image Viewer");
    setTitleBarWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));

    setWidget(renderView_);

    // Surface view title TODO: clean
    auto *surfViewTitleWrapper = new QWidget();
    auto *surfHLayout = new QHBoxLayout();
    surfHLayout->addWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    surfHLayout->addStretch();

#ifdef EASTER_EGG_MODE
    auto *minecraftCheckBox = new QCheckBox("Minecraft");
    QObject::connect(minecraftCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleMinecraft();
    });
    surfHLayout->addWidget(minecraftCheckBox);
#endif

    auto *surfPerspectiveCheckBox = new QCheckBox("Orthographic");
    QObject::connect(surfPerspectiveCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleOrthographic();
    });
    surfHLayout->addWidget(surfPerspectiveCheckBox);

    auto *imageColorsCheckBox = new QCheckBox("Image Colors");
    imageColorsCheckBox->setChecked(true); // TODO: don't hardcode
    QObject::connect(imageColorsCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleImageColors();
    });
    surfHLayout->addWidget(imageColorsCheckBox);


    surfViewTitleWrapper->setLayout(surfHLayout);
    setTitleBarWidget(surfViewTitleWrapper);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
}

nitro::RenderDockWidget::~RenderDockWidget() = default;
