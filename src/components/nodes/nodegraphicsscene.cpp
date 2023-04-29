#include "nodegraphicsscene.hpp"
#include <QAction>
#include <iostream>
#include <QDebug>

nitro::NodeGraphicsScene::NodeGraphicsScene(QtNodes::AbstractGraphModel &graphModel,
                                            QObject *parent) : QtNodes::BasicGraphicsScene(
        graphModel, parent) {
    auto spawnMenu = new QAction("Spawn menu", this);
    spawnMenu->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_A));
    connect(spawnMenu, &QAction::triggered, this, &NodeGraphicsScene::spawnNodeMenu);

}

QMenu *nitro::NodeGraphicsScene::createSceneMenu(const QPointF scenePos) {
    auto *menu = new QMenu();
    auto *inputSubMenu = new QMenu();
    inputSubMenu->addAction("add");
    menu->addMenu(menu);
    return menu;
}

void nitro::NodeGraphicsScene::spawnNodeMenu() {
    qDebug() << "hoi";
}
