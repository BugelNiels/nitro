#include "include/gui/draggabletreewidget.hpp"
#include <QMouseEvent>
#include <QAction>

void nitro::DraggableTreeWidget::mousePressEvent(QMouseEvent *event) {

    QTreeWidget::mousePressEvent(event);
    auto item = itemAt(event->pos());
    if (item && event->button() == Qt::LeftButton) {

        if(actions.contains(item->text(0))) {
            m_draggedItem = item;
            setCursor(Qt::OpenHandCursor);
        }
    }
}

void nitro::DraggableTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);
    if (m_draggedItem && event->button() == Qt::LeftButton) {
        if(actions.contains(m_draggedItem->text(0))) {
            actions[m_draggedItem->text(0)]->trigger();
        }
        m_draggedItem = nullptr;
    }
}

void nitro::DraggableTreeWidget::registerAction(const QString &key, QAction *action) {
    actions[key] = action;
}

void nitro::DraggableTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    if(!m_draggedItem) {
        QTreeWidget::mouseMoveEvent(event);
    }
}
