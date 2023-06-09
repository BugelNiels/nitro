#include "renderdockwidget.hpp"

#include <QHBoxLayout>
#include <QCheckBox>
#include "renderview.hpp"
#include "mainwindow.hpp"

nitro::RenderDockWidget::RenderDockWidget(RenderView *renderView, MainWindow *window)
        : QDockWidget(window),
          renderView_(renderView) {
    setTitleBarWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));

    setWidget(renderView_);

    // Surface view title TODO: clean
    auto *surfViewTitleWrapper = new QWidget();
    auto *surfHLayout = new QHBoxLayout();
    surfHLayout->addWidget(window->buildDockIcon(":/icons/surface_visualizer.png"));
    surfHLayout->addStretch();

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