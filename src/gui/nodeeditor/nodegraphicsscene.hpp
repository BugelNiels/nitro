#pragma once

#include <QtNodes/BasicGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

namespace nitro {

    class NodeGraphicsScene : public QtNodes::BasicGraphicsScene {
    public:
        NodeGraphicsScene(QtNodes::AbstractGraphModel &graphModel, QObject *parent = nullptr);

        NodeGraphicsScene() = delete;

        ~NodeGraphicsScene() override = default;

    protected:

        void keyPressEvent(QKeyEvent *event) override;

        bool event(QEvent *event) override {
            // Hackiest code ever; disables deselecting of nodes when ctrl and shift are pressed
            auto *check = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            if (check && check->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
                return true;
            }
            return QGraphicsScene::event(event); // dispatch event to parent implementation
        }
    private:
        QtNodes::AbstractGraphModel& graphModel_;
    };

} // nitro
