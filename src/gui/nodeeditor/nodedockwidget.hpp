#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include "QtNodes/NodeGraphicsView.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "imgnodegraphicsview.hpp"

namespace nitro {

    class MainWindow;

    class NodeDockWidget : public QDockWidget {
    public:

        NodeDockWidget(NodeRegistry *nodes, MainWindow *window);

        ~NodeDockWidget() override;

        void clearModel();

        bool canQuitSafely();

        void saveModel(bool askFile = false);

        void loadModel();

        const QString &getFileName();

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    private:
        NodeGraphicsView *view_;
        QJsonObject prevSave_;
        QString filename_;
        QString saveFilePath;
        QtNodes::DataFlowGraphModel *dataFlowGraphModel_;
        QTreeWidget *nodeTreeWidget_;
        QLineEdit *searchBar_;
        
        bool searchTreeItem(QTreeWidgetItem *item, const QString &searchText);

        void searchTextChanged(const QString &searchText);

        QTreeWidget *initSideMenu();

        QWidget *initNodeTitleBar(nitro::MainWindow *window);
    };

} // nitro