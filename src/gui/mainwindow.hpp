#pragma once

#include <QMainWindow>
#include "nodeview.hpp"

namespace nitro {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:

    private:
        NodeView* nodeView;
        QMenuBar *initMenuBar();

        QStatusBar *initFooter();
    };

} // nitro