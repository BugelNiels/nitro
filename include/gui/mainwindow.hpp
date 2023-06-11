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
        explicit MainWindow(NodeRegistry* registry, QWidget *parent = nullptr);

        ~MainWindow() override;

    public:
        QLabel *buildDockIcon(const QString &path);

        void finalizeSetup();

        [[nodiscard]] NodeDockWidget *getNodeDock() const;

        void registerDock(QDockWidget *widget);

        void registerNodeDock(NodeDockWidget *widget);

    private:

        QSet<QDockWidget *> widgets;
        const int icSize = 16;
        const QColor icColor = {180, 180, 180};

        QLabel *fileNameLabel = nullptr;
        NodeDockWidget *nodeDock = nullptr;

        QMenuBar *initMenuBar();

        QStatusBar *initFooter();

        QMenu *getFileMenu();

        QMenu *getWindowMenu();

        QSplitter *dockLayout_;
    };

} // nitro