#include "gui/draggabletreewidget.hpp"
#include <QMouseEvent>
#include <QAction>

void nitro::DraggableTreeWidget::mousePressEvent(QMouseEvent *event) {
    QTreeWidget::mousePressEvent(event);
    auto item = itemAt(event->pos());
    if (item && event->button() == Qt::LeftButton) {

        if(actions_.contains(item->text(0))) {
            draggedItem_ = item;
            setCursor(Qt::OpenHandCursor);
        }
    }
}

void nitro::DraggableTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);
    if (draggedItem_ && event->button() == Qt::LeftButton) {
        if(actions_.contains(draggedItem_->text(0))) {
            actions_[draggedItem_->text(0)]->trigger();
        }
        draggedItem_ = nullptr;
    }
    unsetCursor();
}

void nitro::DraggableTreeWidget::registerAction(const QString &key, QAction *action) {
    actions_[key] = action;
}

void nitro::DraggableTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    if(!draggedItem_) {(0, Qt::yellow);
        QTreeWidget::mouseMoveEvent(event);
    }
}
