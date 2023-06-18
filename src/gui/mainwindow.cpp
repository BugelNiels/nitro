#include "gui/mainwindow.hpp"

#include <QMenuBar>
#include <QStatusBar>
#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QSplitter>
#include "src/util/imgresourcereader.hpp"
#include "config.hpp"

#include "src/gui/nodeeditor/nodedockwidget.hpp"
#include "stylepresets.hpp"
#include <QApplication>

nitro::MainWindow::MainWindow(NodeRegistry *registry, QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");
    setWindowIcon(nitro::ImResourceReader::getPixMap(":/icons/nitro.png", {48, 48}, QColor(113, 124, 143)));

    dockLayout_ = new QSplitter(Qt::Horizontal, this);
    dockLayout_->setSizes({this->width() / 2, this->width() / 2}); // Temp fix for equal sizes

    // Node editor, visualizers split
    auto *vertLayout = new QSplitter(Qt::Vertical, this);
    vertLayout->addWidget(dockLayout_);
    vertLayout->addWidget(new NodeDockWidget(registry, this));
    vertLayout->setStretchFactor(0, 1);
    vertLayout->setStretchFactor(1, 1);

    setCentralWidget(vertLayout);

}

void nitro::MainWindow::finalizeSetup() {
    installEventFilter(this);

    setMenuBar(initMenuBar());
    setStatusBar(initFooter());

    const int widgetCount = dockLayout_->count();
    QList<int> initialSizes;
    initialSizes.fill(1, widgetCount);
    dockLayout_->setSizes(initialSizes);

    setWindowState(Qt::WindowMaximized);

}

nitro::MainWindow::~MainWindow() = default;


QStatusBar *nitro::MainWindow::initFooter() {
    auto *versionLabel = new QLabel(QString(" version %1 ").arg(NITRO_VERSION), this);
    fileNameLabel_ = new QLabel(" untitled.json ", this);

    auto *statusBar = new QStatusBar(this);
    statusBar->insertPermanentWidget(0, fileNameLabel_, 1);
    statusBar->insertPermanentWidget(1, versionLabel, 0);
    statusBar->setSizeGripEnabled(false);
    statusBar->setStyleSheet("color: grey;");
    return statusBar;
}


QMenuBar *nitro::MainWindow::initMenuBar() {
    auto *menuBar = new QMenuBar();

    QMenu *fileMenu = getFileMenu();

    menuBar->addMenu(fileMenu);

    auto *editMenu = new QMenu("Edit");
    menuBar->addMenu(editMenu);
    auto *viewMenu = new QMenu("View");
    menuBar->addMenu(viewMenu);


    QMenu *windowMenu = getWindowMenu();
    menuBar->addMenu(windowMenu);

    auto *lightModeToggle = new QAction(menuBar);
    lightModeToggle->setIcon(
            QIcon(nitro::ImResourceReader::getPixMap(":/icons/theme.png", {42, 42}, QColor(128, 128, 128))));
    lightModeToggle->setCheckable(true);
    lightModeToggle->setChecked(false); // default is dark mode
    connect(lightModeToggle, &QAction::toggled, this, [this](bool toggled) {
        if (toggled) {
            nitro::applyStylePreset(nitro::getLightModePalette());
        } else {
            nitro::applyStylePreset(nitro::getDarkModePalette());
        }
        QWidget *topLevelWidget = QApplication::topLevelWidgets().at(0);

        // Recursively update the entire widget hierarchy
        nodeDock_->updateGraphicsView();
        const QList<QWidget *> allWidgets = topLevelWidget->findChildren<QWidget *>();
        for (QWidget *widget: allWidgets) {
            widget->repaint();
        }
    });
    auto *rightBar = new QMenuBar(menuBar);
    rightBar->addAction(lightModeToggle);
    menuBar->setCornerWidget(rightBar);
    return menuBar;
}

QMenu *nitro::MainWindow::getWindowMenu() {
    auto *windowMenu = new QMenu("Window");
    for (auto &dw: widgets_) {
        auto *nodeEditorAction = new QAction(dw->windowTitle(), this);
        nodeEditorAction->setCheckable(true);
        nodeEditorAction->setChecked(!dw->isHidden());
        connect(nodeEditorAction, &QAction::triggered, [dw]() {
            dw->setHidden(!dw->isHidden());
        });
        windowMenu->addAction(nodeEditorAction);
    }
    return windowMenu;
}

QMenu *nitro::MainWindow::getFileMenu() {
    auto *fileMenu = new QMenu("File");

    auto *newAction = new QAction(QStringLiteral("New"), this);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            if (!nodeDock_->canQuitSafely()) {
                return;
            }
            fileNameLabel_->setText(nodeDock_->getFileName());
            nodeDock_->clearModel();
        }
    });
    fileMenu->addAction(newAction);

    auto *openAction = new QAction(QStringLiteral("Open"), this);
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            nodeDock_->loadModel();
            fileNameLabel_->setText(nodeDock_->getFileName());
        }
    });
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();

    auto *saveAction = new QAction(QStringLiteral("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            nodeDock_->saveModel();
            fileNameLabel_->setText(nodeDock_->getFileName());
        }
    });
    fileMenu->addAction(saveAction);

    auto *saveAsAction = new QAction(QStringLiteral("Save As..."), this);
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            nodeDock_->saveModel(true);
            fileNameLabel_->setText(nodeDock_->getFileName());
        }
    });
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();

    auto *quitAction = new QAction(QStringLiteral("Quit"), this);
    quitAction->setShortcutContext(Qt::ApplicationShortcut);
    quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(quitAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            if (!nodeDock_->canQuitSafely()) {
                return;
            }
        }
        exit(EXIT_SUCCESS);
    });
    fileMenu->addAction(quitAction);
    return fileMenu;
}


QLabel *nitro::MainWindow::buildDockIcon(const QString &path) {
    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImResourceReader::getPixMap(path, {icSize_, icSize_}, icColor_));
    return nodeIcon;
}

nitro::NodeDockWidget *nitro::MainWindow::getNodeDock() const {
    return nodeDock_;
}

void nitro::MainWindow::registerNodeDock(nitro::NodeDockWidget *widget) {
    nodeDock_ = widget;
    registerDock(widget);
}

void nitro::MainWindow::registerDock(QDockWidget *widget) {
    widgets_.insert(widget);
    dockLayout_->addWidget(widget);
}
