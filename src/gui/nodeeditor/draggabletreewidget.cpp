#include "draggabletreewidget.hpp"
#include <QMouseEvent>
#include <QAction>

void nitro::DraggableTreeWidget::mousePressEvent(QMouseEvent *event) {
    QTreeWidget::mousePressEvent(event);
    auto item = itemAt(event->pos());
    if (item && event->button() == Qt::LeftButton) {
        m_draggedItem = item;
        setCursor(Qt::OpenHandCursor);
    }

}

void nitro::DraggableTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);
    if (m_draggedItem && event->button() == Qt::LeftButton) {
        actions[m_draggedItem->text(0)]->trigger();
        unsetCursor();
        m_draggedItem = nullptr;
    }
}

void nitro::DraggableTreeWidget::registerAction(const QString &key, QAction *action) {
    actions[key] = action;
}
