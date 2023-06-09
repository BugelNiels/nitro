#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include "external/nodeeditor/include/QtNodes/NodeGraphicsView.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "imgnodegraphicsview.hpp"

namespace nitro {

    class MainWindow;

    class NodeDockWidget : public QDockWidget {
    public:

        NodeDockWidget(NodeRegistry* nodes, MainWindow* window);

        ~NodeDockWidget() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

        const QString &getFileName();

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

        QWidget *initNodeTitleBar(nitro::MainWindow *window);
    };

} // nitro