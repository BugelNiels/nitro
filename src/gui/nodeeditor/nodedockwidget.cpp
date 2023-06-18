#include "nodedockwidget.hpp"

#include <QLabel>
#include <QCheckBox>
#include "imgnodegraphicsview.hpp"

#include "src/util/imgresourcereader.hpp"
#include "gui/draggabletreewidget.hpp"

#include <QKeyEvent>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include "QtNodes/DataFlowGraphModel"
#include "QtNodes/NodeDelegateModelRegistry"
#include "gui/mainwindow.hpp"
#include "QtNodes/internal/WidgetNodePainter.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QProgressBar>

#include <QUndoStack>
#include <QPushButton>

nitro::NodeDockWidget::NodeDockWidget(NodeRegistry *nodes, MainWindow *window)
        : QDockWidget(window),
          filename_("untitled.json") {

    dataFlowGraphModel_ = new QtNodes::DataFlowGraphModel(nodes->getRegistry());
    nodeScene_ = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel_);
    nodeScene_->setNodePainter(std::make_unique<QtNodes::WidgetNodePainter>(QtNodes::WidgetNodePainter()));
    nodeScene_->toggleWidgetMode();
    view_ = new ImageNodeGraphicsView(nodes, nodeScene_, dataFlowGraphModel_, window);

    setWindowTitle("Node Editor");
    prevSave_ = dataFlowGraphModel_->save();

    view_->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto *horLayout = new QSplitter(Qt::Horizontal, this);

    auto *wrapper = new QWidget(horLayout);
    auto *menuVertLayout = new QVBoxLayout();
    searchBar_ = new QLineEdit(this);
    searchBar_->setPlaceholderText("Search");
    searchBar_->adjustSize();
    connect(searchBar_, &QLineEdit::textChanged, this, &NodeDockWidget::searchTextChanged);
    QSize size(searchBar_->height(), searchBar_->height());
    auto *searchLabel = new QLabel();
    searchLabel->setPixmap(nitro::ImResourceReader::getPixMap(":/icons/search.png", size));

    auto *searchHorLayout = new QHBoxLayout();
    searchHorLayout->addWidget(searchLabel);
    searchHorLayout->addWidget(searchBar_);
    menuVertLayout->addLayout(searchHorLayout);

    nodeTreeWidget_ = initSideMenu();
    menuVertLayout->addWidget(nodeTreeWidget_);
    wrapper->setLayout(menuVertLayout);
    horLayout->addWidget(wrapper);
    horLayout->addWidget(view_);

    setTitleBarWidget(initNodeTitleBar(window));


    setWidget(horLayout);

    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
    window->registerNodeDock(this);
}


nitro::NodeDockWidget::~NodeDockWidget() = default;

void nitro::NodeDockWidget::searchTextChanged(const QString &searchText) {
    // Loop through all items in the tree widget
    for (int i = 0; i < nodeTreeWidget_->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topLevelItem = nodeTreeWidget_->topLevelItem(i);
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
    treeWidget->setStyleSheet("QTreeView::item { padding: 5px; }");
    treeWidget->setHeaderHidden(true);
    auto nodeMenu = view_->initNodeMenu();
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
                font.setPixelSize(14);
                item->setFont(0, font);
                item->setIcon(0, subAction->icon());

                category->addChild(item);
                treeWidget->registerAction(subAction->text(), subAction);
            }
        }
    }
    treeWidget->expandAll();
    return treeWidget;
}

void nitro::NodeDockWidget::clearModel() {
    for (const auto &item: dataFlowGraphModel_->allNodeIds()) {
        dataFlowGraphModel_->deleteNode(item);
    }
    nodeScene_->undoStack().clear();
    prevSave_ = dataFlowGraphModel_->save();
}

bool nitro::NodeDockWidget::canQuitSafely() {
    if (prevSave_ == dataFlowGraphModel_->save()) {
        return true;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save changes",
                                  QString("Save changes before closing?\n%1").arg(filename_),
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
    if (askFile || filename_ == "untitled.json") {
        filePath = QFileDialog::getSaveFileName(
                this, "Save NITRO Config", "../data/" + filename_,
                tr("Json Files (*.json)"));
        if (filePath == "") {
            return;
        }
        saveFilePath = filePath;
        filename_ = QFileInfo(filePath).fileName();
    } else {
        filePath = saveFilePath;
    }
    auto saveObject = dataFlowGraphModel_->save();
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
            this, "Load NITRO Config", "../data/",
            tr("Json Files (*.json)"));
    if (filePath == "") {
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    update();
    QFile jsonFile(filePath);
    filename_ = QFileInfo(filePath).fileName();
    saveFilePath = filePath;

    jsonFile.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(jsonFile.readAll());
    clearModel();
    dataFlowGraphModel_->load(doc.object());
    prevSave_ = dataFlowGraphModel_->save();
    QApplication::restoreOverrideCursor();
}

void nitro::NodeDockWidget::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Space:
            if (!searchBar_->hasFocus()) {
                searchBar_->setText("");
            }
            searchBar_->setFocus();
            break;
    }
}

const QString &nitro::NodeDockWidget::getFileName() {
    return filename_;
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

void nitro::NodeDockWidget::updateGraphicsView() {
    for (auto &item: dataFlowGraphModel_->allNodeIds()) {
        dataFlowGraphModel_->nodeUpdated(item);
    }

    nodeTreeWidget_->setStyleSheet("QTreeView::item { padding: 5px; }");
}
