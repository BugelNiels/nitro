#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QSplitter>

namespace nitro {

    class NodeRegistry;

    class NodeDockWidget;

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(NodeRegistry *registry, QWidget *parent = nullptr);

        ~MainWindow() override;

        QLabel *buildDockIcon(const QString &path);

        void finalizeSetup();

        [[nodiscard]] NodeDockWidget *getNodeDock() const;

        void registerDock(QDockWidget *widget);

        void registerNodeDock(NodeDockWidget *widget);

    private:
        const int icSize_ = 16;
        const QColor icColor_ = {148, 148, 148};
        QSet<QDockWidget *> widgets_;
        QLabel *fileNameLabel_ = nullptr;
        NodeDockWidget *nodeDock_ = nullptr;
        QSplitter *dockLayout_ = nullptr;

        QMenuBar *initMenuBar();

        QStatusBar *initFooter();

        QMenu *getFileMenu();

        QMenu *getWindowMenu();

    };

} // nitro
