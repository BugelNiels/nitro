#pragma once

#include <QMainWindow>
#include "src/gui/nodeeditor/nodedockwidget.hpp"

namespace nitro {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    protected:

        bool eventFilter(QObject *obj, QEvent *event) override;

    private slots:

    private:
        QLabel* fileNameLabel = nullptr;
        NodeDockWidget *nodeView;

        QMenuBar *initMenuBar();

        QStatusBar *initFooter();

    };

} // nitro