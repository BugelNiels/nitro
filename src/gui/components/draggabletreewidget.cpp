#include "draggabletreewidget.hpp"

#include <QAction>
#include <QMouseEvent>
#include <QScrollBar>

namespace nitro {

DraggableTreeWidget::DraggableTreeWidget(QWidget *parent) : QTreeWidget(parent) {
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void DraggableTreeWidget::mousePressEvent(QMouseEvent *event) {
    QTreeWidget::mousePressEvent(event);
    auto item = itemAt(event->pos());
    if (item && event->button() == Qt::LeftButton) {
        if (actions_.contains(item->text(0))) {
            draggedItem_ = item;
            setCursor(Qt::OpenHandCursor);
        }
    }
}

void DraggableTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);
    if (draggedItem_ && event->button() == Qt::LeftButton) {
        if (actions_.contains(draggedItem_->text(0))) {
            actions_[draggedItem_->text(0)]->trigger();
        }
        draggedItem_ = nullptr;
    }
    unsetCursor();
}

void DraggableTreeWidget::registerAction(const QString &key, QAction *action) {
    actions_[key] = action;
}

void DraggableTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!draggedItem_) {
        QTreeWidget::mouseMoveEvent(event);
    }
}

} // namespace nitro
