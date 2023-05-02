#include "nodegraphicsscene.hpp"
#include "nitronodepainter.hpp"

#include <QtNodes/NodeDelegateModel>
#include <QKeyEvent>

nitro::NodeGraphicsScene::NodeGraphicsScene(QtNodes::AbstractGraphModel &graphModel, QObject *parent)
        : BasicGraphicsScene(graphModel, parent) {
    setNodePainter(std::make_unique<NitroNodePainter>(NitroNodePainter()));
}
