#pragma once

#include <QMainWindow>

namespace nitro {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:

    private:

    };

} // nitro