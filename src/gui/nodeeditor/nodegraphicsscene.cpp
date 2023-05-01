#include "nodegraphicsscene.hpp"

#include <QKeyEvent>

nitro::NodeGraphicsScene::NodeGraphicsScene(QtNodes::AbstractGraphModel &graphModel, QObject *parent)
        : BasicGraphicsScene(graphModel, parent) {

}
