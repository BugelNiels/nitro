#pragma once

#include <QtNodes/GraphicsView>
#include <QMenu>
#include <QtNodes/DataFlowGraphModel>

namespace nitro {

    class NodeGraphicsView : public QtNodes::GraphicsView {
    public:

        NodeGraphicsView(QtNodes::BasicGraphicsScene *scene,
                         QtNodes::DataFlowGraphModel *model,
                         QWidget *parent);


        virtual QMenu *initNodeMenu() = 0;

    public:
        QtNodes::DataFlowGraphModel *getDataModel() const;

        QtNodes::BasicGraphicsScene *getScene() const;

        QMenu *getNodeMenu();

    protected:
        QtNodes::DataFlowGraphModel *dataModel_ = nullptr;
        QtNodes::BasicGraphicsScene *scene_ = nullptr;


    private:
        QMenu *nodeMenu_ = nullptr;
        void spawnNodeMenu();
    };

} // nitro
