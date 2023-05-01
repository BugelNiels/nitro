#pragma once

#include <QTreeWidgetItem>
#include <QMap>

namespace nitro {

    class DraggableTreeWidget : public QTreeWidget {
    public:
        explicit DraggableTreeWidget(QWidget *parent = nullptr)
                : QTreeWidget(parent) {}

        void registerAction(const QString& key, QAction* action);

    protected:

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        QMap<QString, QAction*> actions;
        bool m_pressed = false;
        QPoint m_startPos;
        QTreeWidgetItem *m_draggedItem{};
    };


} // nitro
