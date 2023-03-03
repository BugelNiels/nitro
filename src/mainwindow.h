#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ProgressListener.h"
#include "imageview.h"
#include "renderview.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class represents the main window.
 */
class MainWindow : public QMainWindow, public ProgressListener {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private slots:
  void on_loadImageButton_pressed();
  void on_quantisizeLevelSpinBox_valueChanged(int arg1);
  void on_sampleButton_pressed();
  void on_methodComboBox_currentIndexChanged(int index);
  void on_df3DCheckBox_toggled(bool checked);
  void on_showComboBox_currentIndexChanged(int index);
  void on_compareButton_pressed();
  void on_compareButton_released();

  void on_fitButton_clicked();

  void on_resetButton_clicked();

  void on_quantisizeButton_clicked();

  void on_compareComboBox_currentIndexChanged(int index);

  void on_computeSDFButton_pressed();

  void progressUpdated(int value) override;

 private:
  DispImgType oldShowSetting;
  void updateRenderImage();
  void updateNumLevels(int value);
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
