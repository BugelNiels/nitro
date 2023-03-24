#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->imageView->addListener(this);
  ui->progressBar->setValue(0);
  ui->progressBar->setEnabled(false);

  ui->viewSettingsGroupBox->setEnabled(false);
  ui->sampleSettingsGroupBox->setEnabled(false);
  ui->imgSettingsGroupBox->setEnabled(false);
  ui->imageInfoGroupBox->setEnabled(false);
  ui->greyValMultiplierSpinBox->setValue(
      ui->imageView->sampleSettings.distMult);
  ui->greyValMultiplierSpinBox->setEnabled(false);

  SampleSettings sampleSettings = ui->imageView->sampleSettings;
  ui->quantisizeLevelSpinBox->setValue(sampleSettings.quantisizeLevel);
}

/**
 * @brief MainWindow::~MainWindow Deconstructs the main window.
 */
MainWindow::~MainWindow() { delete ui; }

void MainWindow::newImage(const QString& name) {
  int index = ui->savedImagesView->count();
  ui->imageView->viewSettings.activeImgIndex = index;
  ui->savedImagesView->addItem(name);
  ui->compareImagesView->addItem(name);
  QModelIndex qIndex = ui->savedImagesView->model()->index(index, 0);
  ui->savedImagesView->setCurrentIndex(qIndex);
  if (index == 0) {
    QModelIndex qIndex = ui->compareImagesView->model()->index(index, 0);
    ui->compareImagesView->setCurrentIndex(qIndex);
  }
  ui->imageView->updateImage();
}

void MainWindow::progressUpdated(int value) {
  if (value == 0) {
    ui->progressBar->setEnabled(true);
  }
  ui->progressBar->setValue(value);
}

void MainWindow::updateRenderImage() {
  ui->imageView->updateImage();
  //  ui->renderView->settings.uniformUpdateRequired = true;
  //  ui->renderView->update();
}

void MainWindow::on_compareButton_pressed() {
  oldShowIdx = ui->imageView->viewSettings.activeImgIndex;
  ui->imageView->viewSettings.activeImgIndex =
      ui->imageView->viewSettings.compareImgIndex;
  ui->imageView->updateImage();
}

void MainWindow::on_compareButton_released() {
  ui->imageView->viewSettings.activeImgIndex = oldShowIdx;
  ui->imageView->updateImage();
}

void MainWindow::on_sampleButton_pressed() {
  ui->imageView->resample();
  ui->sampleButton->setEnabled(false);
  newImage(
      QString("Resampled %1").arg(ui->imageView->sampleSettings.sampleMethod));
}

void MainWindow::on_loadImageButton_pressed() {
  if (!ui->imageView->loadFile(QFileDialog::getOpenFileName(
          this, "Load Image", "../images/",
          tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm)")))) {
    return;
  }
  ui->imageView->viewSettings.compareImgIndex = 0;
  ui->imageView->viewSettings.activeImgIndex = 0;
  ui->savedImagesView->clear();
  const QImage& img = ui->imageView->getActiveDisplayImage();
  ui->depthNumLabel->setText(QString("%1").arg(img.depth()));
  ui->sizeNumLabel->setText(
      QString("%1 x %2").arg(img.width()).arg(img.height()));
  ui->greyNumLabel->setText(QString("%1").arg(1 << img.depth()));
  ui->quantisizeButton->setEnabled(true);
  ui->computeSDFButton->setEnabled(true);
  ui->viewSettingsGroupBox->setEnabled(true);
  ui->imgSettingsGroupBox->setEnabled(true);
  ui->sampleSettingsGroupBox->setEnabled(false);
  ui->imageInfoGroupBox->setEnabled(true);
  ui->imageView->scaleImToFit();
  newImage("Original");
}

void MainWindow::on_quantisizeLevelSpinBox_valueChanged(int value) {
  SampleSettings& sampleSettings = ui->imageView->sampleSettings;
  sampleSettings.quantisizeLevel = value;
  ui->quantisizeButton->setEnabled(true);
}

void MainWindow::on_methodComboBox_currentIndexChanged(int index) {
  SampleSettings& sampleSettings = ui->imageView->sampleSettings;
  sampleSettings.sampleMethod = static_cast<SampleMethod>(index);
  ui->sampleButton->setEnabled(true);
  ui->imageView->updateImage();
}

void MainWindow::on_df3DCheckBox_toggled(bool checked) {
  SampleSettings& sampleSettings = ui->imageView->sampleSettings;
  sampleSettings.use3dDistanceField = checked;
  ui->computeSDFButton->setEnabled(true);
  ui->greyValMultiplierSpinBox->setEnabled(
      !ui->greyValMultiplierSpinBox->isEnabled());
}

void MainWindow::on_fitButton_clicked() { ui->imageView->scaleImToFit(); }

void MainWindow::on_resetButton_clicked() { ui->imageView->resetImScale(); }

void MainWindow::on_quantisizeButton_clicked() {
  ui->imageView->quantisize();
  ui->imageView->updateImage();
  ui->quantisizeButton->setEnabled(false);

  newImage(QString("Quantisized %1")
               .arg(ui->imageView->sampleSettings.quantisizeLevel));
  ui->computeSDFButton->setEnabled(true);
}

void MainWindow::on_computeSDFButton_pressed() {
  ui->computeSDFButton->setDown(true);
  if (!ui->imageView->quantisized &&
      ui->imageView->getOriginalImage().depth() > 5) {
    switch (QMessageBox::question(
        this, tr("CBD Convert"),
        tr("You are attempting to compute the distance field of an image with "
           "more than 32 grey levels. This can take a long time. Do you wish "
           "to continue?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::Cancel)) {
      case QMessageBox::Yes:
        break;
      case QMessageBox::No:
        return;
      case QMessageBox::Cancel:
        return;
      default:
        return;
    }
  }
  ui->imageView->calcDistanceField();
  ui->computeSDFButton->setDown(false);
  ui->computeSDFButton->setEnabled(false);
  ui->sampleSettingsGroupBox->setEnabled(true);
  ui->sampleButton->setEnabled(true);
}

void MainWindow::on_greyValMultiplierSpinBox_valueChanged(double value) {
  if (value != ui->imageView->sampleSettings.distMult) {
    ui->imageView->sampleSettings.distMult = value;
    ui->computeSDFButton->setEnabled(true);
  }
}

void MainWindow::on_savedImagesView_itemSelectionChanged() {
  ui->imageView->viewSettings.activeImgIndex =
      ui->savedImagesView->currentIndex().row();
  ui->imageView->updateImage();
}

void MainWindow::on_compareImagesView_itemSelectionChanged() {
  ui->imageView->viewSettings.compareImgIndex =
      ui->compareImagesView->currentIndex().row();
  ui->imageView->updateImage();
}
