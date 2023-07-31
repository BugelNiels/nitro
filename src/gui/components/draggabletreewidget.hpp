#pragma once

#include <QMap>
#include <QTreeWidget>

namespace nitro {

/**
 * A tree widget that allows for the dragging of its components to other widgets.
 */
class DraggableTreeWidget : public QTreeWidget {
public:
    /**
     * Creates a new empty tree widget.
     * @param parent Parent widget.
     */
    explicit DraggableTreeWidget(QWidget *parent = nullptr);

    /**
     * Registers a new action to be associated with a particular item in the tree.
     * @param key A unique id of the action. Should correspond to the text of the item that should trigger this action.
     * @param action The action to be triggered when the item is dropped at the end of a drag and drop.
     */
    void registerAction(const QString &key, QAction *action);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QMap<QString, QAction *> actions_;
    QTreeWidgetItem *draggedItem_ = nullptr;
};

} // namespace nitro
