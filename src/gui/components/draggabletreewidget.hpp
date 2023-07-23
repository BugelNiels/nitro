#pragma once

#include <QMap>
#include <QTreeWidget>

namespace nitro {

class DraggableTreeWidget : public QTreeWidget {
public:
    explicit DraggableTreeWidget(QWidget *parent = nullptr);

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
