#pragma once

#include <QDockWidget>
#include "src/gui/imgviewer/imgviewer.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "nodes/nodegraphicsview.hpp"

namespace nitro {

    class NodeView : public QDockWidget {
    public:

        NodeView(nitro::ImageViewer *imViewer, QWidget *parent);

        ~NodeView() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

        void forwardKeyPress(QKeyEvent *event);

        const QString &getFileName() const {
            return filename;
        }

    private:
        NodeGraphicsView *view;
        QJsonObject prevSave;
        QString filename;
        QString saveFilePath;
        QtNodes::DataFlowGraphModel *dataFlowGraphModel;

    };

} // nitro