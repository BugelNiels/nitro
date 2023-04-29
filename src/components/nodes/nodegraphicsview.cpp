#include "nodegraphicsview.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "nodes/imagesourcedatamodel.hpp"
#include "nodes/tograyscaledatamodel.hpp"
#include "nodes/operators/thresholddatamodel.hpp"
#include <QAction>
#include <QMenu>

nitro::NodeGraphicsView::NodeGraphicsView(QWidget *parent) : GraphicsView(parent) {

}

nitro::NodeGraphicsView::NodeGraphicsView(QtNodes::BasicGraphicsScene *scene, QtNodes::DataFlowGraphModel *model,
                                          QWidget *parent) : GraphicsView(scene,
                                                                          parent), _dataModel(model) {

    auto *spawnMenu = new QAction(QStringLiteral("Add"), this);
    spawnMenu->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
    spawnMenu->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_A));
    connect(spawnMenu,
            &QAction::triggered,
            this,
            &NodeGraphicsView::spawnNodeMenu);

    addAction(spawnMenu);

    _nodeMenu = initNodeMenu();
}

QAction *nitro::NodeGraphicsView::spawnNodeAction(const QString &menuName, const QString &nodeType) {
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        // Mouse position in scene coordinates.
        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = _dataModel->addNode(nodeType);
        _dataModel->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    return createNodeAction;
}

// TODO: check pointer usage
QMenu *nitro::NodeGraphicsView::initInputSubMenu() {
    auto *inputMenu = new QMenu("Input");
    inputMenu->addAction(spawnNodeAction(nitro::ImageSourceDataModel::nodeCaption(),
                                         nitro::ImageSourceDataModel::nodeName()));
    return inputMenu;
}

QMenu *nitro::NodeGraphicsView::initConversionsSubMenu() {
    auto *convertMenu = new QMenu("Conversions");
    convertMenu->addAction(spawnNodeAction(nitro::ToGrayScaleDataModel::nodeCaption(),
                                           nitro::ToGrayScaleDataModel::nodeName()));
    return convertMenu;
}

QMenu *nitro::NodeGraphicsView::initOperationsSubMenu() {
    auto *opsMenu = new QMenu("Operations");
    opsMenu->addAction(spawnNodeAction(nitro::ThresholdDataModel::nodeCaption(),
                                       nitro::ThresholdDataModel::nodeName()));
    return opsMenu;
}

QMenu *nitro::NodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);
    auto *convertMenu = initConversionsSubMenu();
    auto *inputMenu = initInputSubMenu();
    auto *opsMenu = initOperationsSubMenu();

    QAction *sectionTitle = menu->addSection("Add");
    QFont font;
    font.setWeight(QFont::Light);
    font.setPixelSize(10);
    sectionTitle->setFont(font);

    menu->addSeparator();
    menu->addMenu(inputMenu);
    menu->addMenu(opsMenu);
    menu->addMenu(convertMenu);
    menu->setMaximumSize(menu->sizeHint());
    return menu;
}

void nitro::NodeGraphicsView::spawnNodeMenu() {
    _nodeMenu->popup(QCursor::pos());
}
