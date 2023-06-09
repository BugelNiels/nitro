#include "mainwindow.hpp"
#include <QMenuBar>
#include <QStatusBar>

#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QSplitter>
#include "util/imgresourcereader.hpp"
#include "config.hpp"

nitro::MainWindow::MainWindow(NodeRegistry *registry, QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("NITRO");

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
    setWindowState(Qt::WindowMaximized);

}

nitro::MainWindow::~MainWindow() = default;


QStatusBar *nitro::MainWindow::initFooter() {
    auto *versionLabel = new QLabel(QString(" version %1 ").arg(nitro::config::version), this);
    fileNameLabel = new QLabel(" untitled.json ", this);

    auto *statusBar = new QStatusBar(this);
    statusBar->insertPermanentWidget(0, fileNameLabel, 1);
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
    // TODO: extract this color
    menuBar->setStyleSheet("QMenuBar { background-color: rgb(28, 28, 28); }");
    return menuBar;
}

QMenu *nitro::MainWindow::getWindowMenu() {
    auto *windowMenu = new QMenu("Window");
    for (auto &dw: widgets) {
        auto *nodeEditorAction = new QAction(dw->windowTitle(), this);
        nodeEditorAction->setCheckable(true);
        nodeEditorAction->setChecked(!dw->isHidden());
        connect(nodeEditorAction, &QAction::triggered, [this, dw]() {
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


QLabel *nitro::MainWindow::buildDockIcon(const QString &path) {
    auto *nodeIcon = new QLabel();
    nodeIcon->setPixmap(ImgResourceReader::getPixMap(path, {icSize, icSize}, icColor));
    return nodeIcon;
}

nitro::NodeDockWidget *nitro::MainWindow::getNodeDock() const {
    return nodeDock;
}

void nitro::MainWindow::registerNodeDock(nitro::NodeDockWidget *widget) {
    nodeDock = widget;
    registerDock(widget);
}

void nitro::MainWindow::registerDock(QDockWidget *widget) {
    widgets.insert(widget);
    dockLayout_->addWidget(widget);
    dockLayout_->setStretchFactor(widgets.size() - 1, 1);
}
