#pragma once

#include <QDockWidget>
#include "src/gui/imgviewer/imgviewer.hpp"
#include "QtNodes/DataFlowGraphModel"

namespace nitro {

    class NodeView : public QDockWidget {
    public:

        NodeView(nitro::ImageViewer *imViewer, QWidget *parent);

        ~NodeView() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

    private:
        QJsonObject prevSave;
        QString filename;
        QString saveFilePath;
        QtNodes::DataFlowGraphModel* dataFlowGraphModel;
    };

} // nitro