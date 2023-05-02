#include "nodedockwidget.hpp"
#include "src/nodes/input/imagesourcedatamodel.hpp"
#include "src/nodes/conversions/tograyscaledatamodel.hpp"
#include "src/nodes/conversions/seperatergbdatamodel.hpp"
#include "src/nodes/operators/thresholddatamodel.hpp"
#include "nodegraphicsview.hpp"
#include "src/nodes/output/imageviewerdatamodel.hpp"
#include "src/nodes/operators/quantization/kmeansdatamodel.hpp"
#include "src/nodes/operators/quantization/quantisizedatamodel.hpp"
#include "src/nodes/operators/flipdatamodel.hpp"
#include "src/nodes/operators/math/imgmathdatamodel.hpp"
#include "src/nodes/operators/reconstruction/resampledatamodel.hpp"
#include "nodegraphicsscene.hpp"
#include "util/imgresourcereader.hpp"
#include "draggabletreewidget.hpp"

#include <QKeyEvent>
#include <QtGui/QScreen>
#include <QtNodes/BasicGraphicsScene>
#include <QtWidgets/QApplication>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLineEdit>

static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels() {
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    ret->registerModel<nitro::ImageSourceDataModel>("Input");
    ret->registerModel<nitro::ImageViewerDataModel>("Output");
    ret->registerModel<nitro::ToGrayScaleDataModel>("Converter");
    ret->registerModel<nitro::ThresholdDataModel>("Operator");
    ret->registerModel<nitro::KMeansDataModel>("Operator");
    ret->registerModel<nitro::QuantisizeDataModel>("Operator");
    ret->registerModel<nitro::FlipDataModel>("Operator");
    ret->registerModel<nitro::ResampleDataModel>("Operator");
    ret->registerModel<nitro::SeperateRgbDataModel>("Operator");
    ret->registerModel<nitro::ImgMathDataModel>("Operator");

    return ret;
}

nitro::NodeDockWidget::NodeDockWidget(nitro::ImageViewer *imViewer, QWidget *parent) : QDockWidget(parent),
                                                                                       filename("untitled.json") {
    setWindowTitle("Node Editor");
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    dataFlowGraphModel->addNode(nitro::ImageSourceDataModel::nodeName());
    nodeScene = new nitro::NodeGraphicsScene(*dataFlowGraphModel);
    view = new nitro::NodeGraphicsView(imViewer, nodeScene, dataFlowGraphModel, this);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    prevSave = dataFlowGraphModel->save();


    auto *horLayout = new QSplitter(Qt::Horizontal, this);

    auto *wrapper = new QWidget(horLayout);
    auto *menuVertLayout = new QVBoxLayout();
    searchBar = new QLineEdit(this);

    searchBar->setPlaceholderText("Search");
    nodeTreeWidget = initSideMenu();
    connect(searchBar, &QLineEdit::textChanged, this, &NodeDockWidget::searchTextChanged);

    menuVertLayout->addWidget(searchBar);
    menuVertLayout->addWidget(nodeTreeWidget);
    wrapper->setLayout(menuVertLayout);
    horLayout->addWidget(wrapper);
    horLayout->addWidget(view);


    setWidget(horLayout);
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
    if (dataFlowGraphModel) {
        for (const auto &item: dataFlowGraphModel->allNodeIds()) {
            dataFlowGraphModel->deleteNode(item);
        }
        // TODO: delete undo history
    }
    prevSave = dataFlowGraphModel->save();
}

bool nitro::NodeDockWidget::canQuitSafely() {
    if (prevSave == dataFlowGraphModel->save()) {
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

    if (!dataFlowGraphModel) {
        return;
    }
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
        prevSave = saveObject;
    } else {
        QMessageBox::warning(this, "Failed to open file", filePath);
    }
}

void nitro::NodeDockWidget::loadModel() {
    if (!dataFlowGraphModel) {
        return;
    }
    if (!canQuitSafely()) {
        return;
    }
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load NITRO Config", "../saves/",
            tr("Json Files (*.json)"));
    if (filePath == "") {
        return;
    }
    QFile jsonFile(filePath);
    filename = QFileInfo(filePath).fileName();

    jsonFile.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(jsonFile.readAll());
    clearModel();
    dataFlowGraphModel->load(doc.object());
    prevSave = dataFlowGraphModel->save();
    // Ensure we cannot create a second viewer
    for (auto &c: dataFlowGraphModel->allNodeIds()) {
        auto attempt = dataFlowGraphModel->delegateModel<nitro::ImageViewerDataModel>(c);
        if (attempt) {
            view->setViewerNodeId(c);
            break;
        }
    }
}

void nitro::NodeDockWidget::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Space:
            if(!searchBar->hasFocus()) {
                searchBar->setText("");
            }
            searchBar->setFocus();
            break;
    }
}

void nitro::NodeDockWidget::recalculateNodeSizes() {
    for(auto& o : dataFlowGraphModel->allNodeIds()) {
        nodeScene->onNodeUpdated(o);
    }
}
