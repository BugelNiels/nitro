#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeGraphicsView.hpp>

#include "imgnodegraphicsview.hpp"

namespace nitro {

class MainWindow;

/**
 * @brief A dock widget used for the node editor view.
 */
class NodeDockWidget : public QDockWidget {
public:
    /**
     * @brief Creates a new node dock with the corresponding view from the provided node registry and adds it to the MainWindow.
     * @param nodes The node registry containing all nodes to add.
     * @param window The main window this dock should be added to.
     */
    NodeDockWidget(std::shared_ptr<NodeRegistry> &nodes, MainWindow *window);

    /**
     * @brief Destructor.
     */
    ~NodeDockWidget() override;

    /**
     * @brief Clears the graph model and removes all the nodes.
     */
    void clearModel();

    /**
     * @brief Checks whether the user can safely quit from the application without losing unsaved changes.
     * @return True if the user can quit safely; false otherwise.
     */
    bool canQuitSafely();

    /**
     * @brief Attempts to save the node graph. If the model has not been saved yet, it will always ask the user for a file location.
     * @param askFile Whether to ask for a file location or not.
     */
    void saveModel(bool askFile = false);

    /**
     * Load a node graph. Will ask the user to locate a file on their disk.
     */
    void loadModel();

    /**
     * @brief Retrieves the file name of the saved file.
     * @return The file name of the saved file.
     */
    const QString &getFileName();

    /**
     * @brief Refreshes the entire node graph view.
     */
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
