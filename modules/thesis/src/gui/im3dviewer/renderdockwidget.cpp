#include "renderdockwidget.hpp"

#include "renderview.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <gui/mainwindow.hpp>

#define EASTER_EGG_MODE

namespace nitro::Thesis {

RenderDockWidget::RenderDockWidget(RenderView *renderView, MainWindow *window)
    : NitroDockWidget("3D Image Viewer",window),
      renderView_(renderView) {

    setIcon(":/icons/surface_visualizer.png");
    setWidget(renderView_);

#ifdef EASTER_EGG_MODE
    auto *minecraftCheckBox = new QCheckBox("Minecraft");
    QObject::connect(minecraftCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleMinecraft();
    });
    addTitleBarWidget(minecraftCheckBox);
#endif

    auto *surfPerspectiveCheckBox = new QCheckBox("Orthographic");
    QObject::connect(surfPerspectiveCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleOrthographic();
    });
    addTitleBarWidget(surfPerspectiveCheckBox);

    auto *imageColorsCheckBox = new QCheckBox("Image Colors");
    imageColorsCheckBox->setChecked(true); // TODO: don't hardcode
    QObject::connect(imageColorsCheckBox, &QCheckBox::toggled, window, [&] {
        renderView_->toggleImageColors();
    });
    addTitleBarWidget(imageColorsCheckBox);
}

RenderDockWidget::~RenderDockWidget() = default;

} // namespace nitro::Thesis
