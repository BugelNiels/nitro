#pragma once

#include <QDockWidget>
#include "src/gui/imgviewer/imgviewer.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "nodegraphicsview.hpp"

namespace nitro {

    class NodeDockWidget : public QDockWidget {
    public:

        NodeDockWidget(nitro::ImageViewer *imViewer, QWidget *parent);

        ~NodeDockWidget() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

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