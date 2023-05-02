#pragma once

#include <QMainWindow>
#include "src/gui/nodeeditor/nodedockwidget.hpp"

namespace nitro {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:

    private:
        const int icSize = 16;
        const int icMargin = 5;
        const QColor icColor = {180, 180, 180};

        QLabel *fileNameLabel = nullptr;
        NodeDockWidget *nodeDock;

        QMenuBar *initMenuBar();

        QStatusBar *initFooter();

        QWidget *initNodeTitleBar();
    };

} // nitro