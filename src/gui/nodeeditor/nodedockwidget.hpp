#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeGraphicsView.hpp>

#include "imgnodegraphicsview.hpp"

namespace nitro {

class MainWindow;

class NodeDockWidget : public QDockWidget {
public:
    NodeDockWidget(std::shared_ptr<NodeRegistry> &nodes, MainWindow *window);

    ~NodeDockWidget() override;

    void clearModel();

    bool canQuitSafely();

    void saveModel(bool askFile = false);

    void loadModel();

    const QString &getFileName();

    void updateGraphicsView();

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
    QtNodes::BasicGraphicsScene *nodeScene_;

    bool searchTreeItem(QTreeWidgetItem *item, const QString &searchText);

    void searchTextChanged(const QString &searchText);

    QTreeWidget *initSideMenu();

    QWidget *initNodeTitleBar(nitro::MainWindow *window);
};

} // namespace nitro
