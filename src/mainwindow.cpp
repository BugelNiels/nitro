#include "mainwindow.h"

#include <QFileDialog>

#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

/**
 * @brief MainWindow::~MainWindow Deconstructs the main window.
 */
MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_pressed() {
  ui->imageView->loadFile(QFileDialog::getOpenFileName(
      this, "Load Image", "../images/",
      tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm)")));
}
