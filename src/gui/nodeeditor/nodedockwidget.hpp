#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include "nodegraphicsview.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "src/improc/ui/imgnodegraphicsview.hpp"
#include "src/gui/nodeeditor/nodegraphicsscene.hpp"

namespace nitro {

    class NodeDockWidget : public QDockWidget {
    public:

        NodeDockWidget(NodeGraphicsView *view, QWidget *parent);

        ~NodeDockWidget() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

        const QString &getFileName();

        void recalculateNodeSizes();

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:
        NodeGraphicsView *view;
        QJsonObject prevSave_;
        QString filename;
        QString saveFilePath;
        QtNodes::DataFlowGraphModel *dataFlowGraphModel;

        QTreeWidget *initSideMenu();

        bool searchTreeItem(QTreeWidgetItem *item, const QString &searchText);

        void searchTextChanged(const QString &searchText);

        QTreeWidget *nodeTreeWidget;
        QLineEdit *searchBar;

        QWidget *initNodeTitleBar();
    };

} // nitro