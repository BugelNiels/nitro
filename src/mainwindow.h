#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "imageview.h"
#include "meshview.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class represents the main window.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private slots:
  void on_pushButton_pressed();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
