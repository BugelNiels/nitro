#include <gui/mainwindow.hpp>

#include <QMenuBar>
#include <QStatusBar>
#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QSplitter>
#include <QApplication>
#include <config.hpp>

#include "src/util/imgresourcereader.hpp"
#include "src/gui/nodeeditor/nodedockwidget.hpp"
#include "stylepresets.hpp"

namespace nitro {

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");
    setWindowIcon(QIcon(":/icons/nitro.png"));

    dockLayout_ = new QSplitter(Qt::Horizontal, this);
    dockLayout_->setProperty("handleWidth", 16);

    // Node editor, visualizers split
    vertLayout_ = new QSplitter(Qt::Vertical, this);
    vertLayout_->setProperty("handleWidth", 16);
    vertLayout_->addWidget(dockLayout_);

}

void MainWindow::finalizeSetup(NodeRegistry *registry) {
    vertLayout_->addWidget(new NodeDockWidget(registry, this));
    setCentralWidget(vertLayout_);
    installEventFilter(this);

    setMenuBar(initMenuBar());
    setStatusBar(initFooter());

    setWindowState(Qt::WindowMaximized);

    finalized_ = true;
}

MainWindow::~MainWindow() = default;


QStatusBar *MainWindow::initFooter() {
    auto *versionLabel = new QLabel(QString(" version %1 ").arg(NITRO_VERSION), this);
    fileNameLabel_ = new QLabel(" untitled.json ", this);

    auto *statusBar = new QStatusBar(this);
    statusBar->insertPermanentWidget(0, fileNameLabel_, 1);
    statusBar->insertPermanentWidget(1, versionLabel, 0);
    statusBar->setSizeGripEnabled(false);
    statusBar->setStyleSheet("color: grey;");
    return statusBar;
}


QMenuBar *MainWindow::initMenuBar() {
    auto *menuBar = new QMenuBar();
    auto *iconLabel = new QAction(QIcon(":/icons/nitro.png"), "", this);
    menuBar->addAction(iconLabel);
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
            QIcon(ImResourceReader::getPixMap(":/icons/theme.png", {42, 42}, QColor(128, 128, 128))));
    lightModeToggle->setCheckable(true);
    lightModeToggle->setChecked(false); // default is dark mode
    connect(lightModeToggle, &QAction::toggled, this, [this](bool toggled) {
        if (toggled) {
            applyStylePreset(getLightModePalette());
        } else {
            applyStylePreset(getDarkModePalette());
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

QMenu *MainWindow::getWindowMenu() {
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

QMenu *MainWindow::getFileMenu() {
    auto *fileMenu = new QMenu("File");

    auto *newAction = new QAction(QStringLiteral("New"), this);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            QApplication::restoreOverrideCursor();
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
            QApplication::restoreOverrideCursor();
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
            QApplication::restoreOverrideCursor();
            nodeDock_->saveModel();
            fileNameLabel_->setText(nodeDock_->getFileName());
        }
    });
    fileMenu->addAction(saveAction);

    auto *saveAsAction = new QAction(QStringLiteral("Save As..."), this);
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, [this]() {
        if (nodeDock_) {
            QApplication::restoreOverrideCursor();
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
            QApplication::restoreOverrideCursor();
            if (!nodeDock_->canQuitSafely()) {
                return;
            }
        }
        exit(EXIT_SUCCESS);
    });
    fileMenu->addAction(quitAction);
    return fileMenu;
}


QLabel *MainWindow::buildDockIcon(const QString &path) {
    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImResourceReader::getPixMap(path, {icSize_, icSize_}, icColor_));
    return nodeIcon;
}

void MainWindow::registerNodeDock(NodeDockWidget *widget) {
    nodeDock_ = widget;
    registerDock(widget);
}

void MainWindow::registerDock(QDockWidget *widget) {
    if (!isFinalized()) {
        return;
    }
    widgets_.push_back(widget);
    dockLayout_->addWidget(widget);
    if (widgets_.size() == 1) {
        vertLayout_->setSizes({1, 1});
    }
}

bool MainWindow::isFinalized() const {
    return finalized_;
}

} // namespace nitro
