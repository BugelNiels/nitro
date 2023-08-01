#include "gui/nitrodockwidget.hpp"
#include "src/util/imgresourcereader.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QTabBar>
#include <gui/mainwindow.hpp>

namespace nitro {

const int SPACING = 10;

NitroDockWidget::NitroDockWidget(const QString &title, MainWindow *window)
    : QDockWidget(window),
      window_(window),
      title_(title) {
    setWindowTitle(title_);

    QWidget *imViewTitleWrapper = initTitleBarWidget();

    setTitleBarWidget(imViewTitleWrapper);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
}

NitroDockWidget::~NitroDockWidget() {
    if (dockFrame_ != nullptr) {
        setParent(nullptr);
    }
}

QWidget *NitroDockWidget::initTitleBarWidget() {
    auto imViewTitleWrapper = new QWidget();
    titleBarLayout_ = new QHBoxLayout();
    titleBarLayout_->addStretch();

    auto *dockButton = new QPushButton("");
    dockButton->setIcon(nitro::ImResourceReader::getPixMap(":/icons/undock.png"));
    dockButton->setToolTip("Press to dock/undock this widget.");
    connect(dockButton, &QPushButton::clicked, this, [this, dockButton](bool checked) {
        if (!docked) {
            dockFrame_->close();
            window_->registerDock(this);
            dockButton->setIcon(nitro::ImResourceReader::getPixMap(":/icons/undock.png"));
        } else {
            dockFrame_ = std::make_unique<QDialog>(window_, Qt::Window);
            dockFrame_->setWindowTitle(title_);
            auto *dialogLayout = new QHBoxLayout();
            dialogLayout->addWidget(this);
            dockFrame_->setLayout(dialogLayout);
            dockFrame_->setVisible(true);
            dockButton->setIcon(nitro::ImResourceReader::getPixMap(":/icons/dock.png"));
        }
        docked = !docked;
    });
    titleBarLayout_->addWidget(dockButton);

    imViewTitleWrapper->setLayout(titleBarLayout_);
    return imViewTitleWrapper;
}

QWidget *NitroDockWidget::spacing() const {
    auto label = new QLabel("|");
    label->setContentsMargins(SPACING, 0, SPACING, 0);
    return label;
}
void NitroDockWidget::setIcon(const QString &path) {
    titleBarLayout_->insertWidget(0, window_->buildDockIcon(path));
}
void NitroDockWidget::addTitleBarWidget(QWidget *widget) {
    titleBarLayout_->insertWidget(wIdx, widget);
    wIdx++;
}

} // namespace nitro