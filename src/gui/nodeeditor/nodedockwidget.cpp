#include "nodedockwidget.hpp"

#include <QLabel>
#include <QCheckBox>
#include "imgnodegraphicsview.hpp"

#include "util/imgresourcereader.hpp"
#include "src/gui/components/draggabletreewidget.hpp"

#include <QKeyEvent>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include "external/nodeeditor/include/QtNodes/DataFlowGraphModel"
#include "external/nodeeditor/include/QtNodes/NodeDelegateModelRegistry"
#include "config.hpp"
#include "mainwindow.hpp"
#include "QtNodes/internal/WidgetNodePainter.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QProgressBar>


nitro::NodeDockWidget::NodeDockWidget(NodeRegistry *nodes, MainWindow *window)
        : QDockWidget(window),
          filename("untitled.json") {

    dataFlowGraphModel = new QtNodes::DataFlowGraphModel(nodes->getRegistry());
    auto *nodeScene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);
    nodeScene->setNodePainter(std::make_unique<QtNodes::WidgetNodePainter>(QtNodes::WidgetNodePainter()));
    nodeScene->toggleWidgetMode();
    view = new ImageNodeGraphicsView(nodes, nodeScene, dataFlowGraphModel, window);

    setWindowTitle("Node Editor");
    prevSave_ = dataFlowGraphModel->save();

    view->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto *horLayout = new QSplitter(Qt::Horizontal, this);

    auto *wrapper = new QWidget(horLayout);
    auto *menuVertLayout = new QVBoxLayout();


    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search");
    searchBar->adjustSize();
    connect(searchBar, &QLineEdit::textChanged, this, &NodeDockWidget::searchTextChanged);
    QSize size(searchBar->height(), searchBar->height());
    auto *searchLabel = new QLabel();
    searchLabel->setPixmap(nitro::ImgResourceReader::getPixMap(":/icons/search.png", size));

    auto *searchHorLayout = new QHBoxLayout();
    searchHorLayout->addWidget(searchLabel);
    searchHorLayout->addWidget(searchBar);
    menuVertLayout->addLayout(searchHorLayout);


    nodeTreeWidget = initSideMenu();
    menuVertLayout->addWidget(nodeTreeWidget);
    wrapper->setLayout(menuVertLayout);
    horLayout->addWidget(wrapper);
    horLayout->addWidget(view);

    setTitleBarWidget(initNodeTitleBar(window));


    setWidget(horLayout);

    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerNodeDock(this);
}


nitro::NodeDockWidget::~NodeDockWidget() = default;

void nitro::NodeDockWidget::searchTextChanged(const QString &searchText) {
    // Loop through all items in the tree widget
    for (int i = 0; i < nodeTreeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topLevelItem = nodeTreeWidget->topLevelItem(i);
        int matchCount = 0;
        for (int j = 0; j < topLevelItem->childCount(); ++j) {
            matchCount += searchTreeItem(topLevelItem->child(j), searchText);
        }
        topLevelItem->setHidden(matchCount == topLevelItem->childCount());
    }
}

inline bool isFuzzyMatch(const QString &str1, const QString &str2) {
    return str1.toLower().contains(str2.toLower()) ||
           QString::localeAwareCompare(str1, str2) == 0;
}

bool nitro::NodeDockWidget::searchTreeItem(QTreeWidgetItem *item, const QString &searchText) {
    // Hide/show the item based on whether it matches the search text
    bool matches = isFuzzyMatch(item->text(0), searchText);
    item->setHidden(!matches);
    return !matches;
}

QTreeWidget *nitro::NodeDockWidget::initSideMenu() {
    auto *treeWidget = new nitro::DraggableTreeWidget(this);
    treeWidget->setHeaderHidden(true);
    auto nodeMenu = view->initNodeMenu();
    for (auto *subMen: nodeMenu->actions()) {

        if (subMen->menu()) {
            auto *category = new QTreeWidgetItem(treeWidget, QStringList() << subMen->text());
            for (auto *subAction: subMen->menu()->actions()) {
                if (subAction->isSeparator()) {
                    continue;
                }
                auto *item = new QTreeWidgetItem(category, QStringList() << subAction->text());
                auto font = category->font(0);
                font.setWeight(QFont::Light);
                font.setPixelSize(14); // TODO: more dynamic
                item->setForeground(0, QBrush({180, 180, 180}));
                item->setFont(0, font);
                item->setIcon(0, subAction->icon());

                category->addChild(item);
                treeWidget->registerAction(subAction->text(), subAction);
                // TODO: allow drag and drop
            }
        }
    }
    treeWidget->expandAll();
    return treeWidget;
}

void nitro::NodeDockWidget::clearModel() {
    for (const auto &item: dataFlowGraphModel->allNodeIds()) {
        dataFlowGraphModel->deleteNode(item);
    }
    // TODO: delete undo history

    prevSave_ = dataFlowGraphModel->save();
}

bool nitro::NodeDockWidget::canQuitSafely() {
    if (prevSave_ == dataFlowGraphModel->save()) {
        return true;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save changes before closing?", filename,
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        saveModel();
    } else if (reply == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void nitro::NodeDockWidget::saveModel(bool askFile) {
    QString filePath;
    if (askFile || filename == "untitled.json") {
        filePath = QFileDialog::getSaveFileName(
                this, "Save NITRO Config", "../saves/" + filename,
                tr("Json Files (*.json)"));
        if (filePath == "") {
            return;
        }
        saveFilePath = filePath;
        filename = QFileInfo(filePath).fileName();
    } else {
        filePath = saveFilePath;
    }
    auto saveObject = dataFlowGraphModel->save();
    QFile jsonFile(filePath);
    if (jsonFile.open(QFile::WriteOnly)) {
        QJsonDocument document;
        document.setObject(saveObject);
        jsonFile.write(document.toJson());
        prevSave_ = saveObject;
    } else {
        QMessageBox::warning(this, "Failed to open file", filePath);
    }
}

void nitro::NodeDockWidget::loadModel() {
    if (!canQuitSafely()) {
        return;
    }
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load NITRO Config", "../saves/",
            tr("Json Files (*.json)"));
    if (filePath == "") {
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    update();
    QFile jsonFile(filePath);
    filename = QFileInfo(filePath).fileName();
    saveFilePath = filePath;

    jsonFile.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(jsonFile.readAll());
    clearModel();
    dataFlowGraphModel->load(doc.object());
    prevSave_ = dataFlowGraphModel->save();
    QApplication::restoreOverrideCursor();
}

void nitro::NodeDockWidget::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Space:
            if (!searchBar->hasFocus()) {
                searchBar->setText("");
            }
            searchBar->setFocus();
            break;
    }
}

const QString &nitro::NodeDockWidget::getFileName() {
    return filename;
}

QWidget *nitro::NodeDockWidget::initNodeTitleBar(nitro::MainWindow *window) {
    auto *wrapper = new QWidget();
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(window->buildDockIcon(":/icons/node_editor.png"));

    hLayout->addSpacing(15);
    hLayout->addStretch();

    auto *calcProgressBar = new QProgressBar();
    calcProgressBar->setMinimum(0);
    calcProgressBar->setMaximum(100);
    calcProgressBar->setMaximumWidth(200);
    hLayout->addWidget(calcProgressBar);

    wrapper->setLayout(hLayout);
    return wrapper;
}
