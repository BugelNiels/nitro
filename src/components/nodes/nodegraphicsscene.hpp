#pragma once

#include <QtNodes/BasicGraphicsScene>

namespace nitro {

    class NodeGraphicsScene : public QtNodes::BasicGraphicsScene {
    public:
        NodeGraphicsScene(QtNodes::AbstractGraphModel
        &graphModel,
        QObject *parent = nullptr
        );

        ~NodeGraphicsScene() = default;


        QMenu *createSceneMenu(QPointF const scenePos) override;
    public Q_SLOTS:
        void spawnNodeMenu();
    };

} // nitro
