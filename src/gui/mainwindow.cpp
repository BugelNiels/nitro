#include "mainwindow.hpp"
#include "nodeview.hpp"
#include "imageview.hpp"
#include <QFileDialog>
#include <QMessageBox>

#include "util/imgconvert.hpp"

/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("NITRO");

    auto* imDock = new QDockWidget("Image Viewer",this);
    auto* imView = new nitro::ImageView(imDock);
    imDock->setWidget(imView);
    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, imDock);
    imDock->setMinimumHeight(400);


    auto* surfDock = new QDockWidget("Surface Visualizer",this);
//    auto* surfView = new nitro::RenderView(imDock);
//    surfDock->setWidget(surfView);
    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, surfDock);

    setCentralWidget(new nitro::NodeView(imView, this));
    setFocus();
}

MainWindow::~MainWindow() {  }

//void MainWindow::newImage(const QString& name) {
//  int index = ui->savedImagesView->count();
//  qDebug() << index;
//  ui->imageView->viewSettings.activeImgIndex = index;
//  ui->savedImagesView->addItem(name);
//  ui->compareImagesView->addItem(name);
//  QModelIndex qIndex = ui->savedImagesView->model()->index(index, 0);
//  ui->savedImagesView->setCurrentIndex(qIndex);
//  if (index == 0) {
//    QModelIndex qIndex = ui->compareImagesView->model()->index(index, 0);
//    ui->compareImagesView->setCurrentIndex(qIndex);
//  }
//  ui->imageView->updateImage();
//}
//
//void MainWindow::updateRenderImage() {
//  ui->imageView->updateImage();
//  //  ui->renderView->settings.uniformUpdateRequired = true;
//  //  ui->renderView->update();
//}
//
//void MainWindow::on_compareButton_pressed() {
//  oldShowIdx = ui->imageView->viewSettings.activeImgIndex;
//  ui->imageView->viewSettings.activeImgIndex =
//      ui->imageView->viewSettings.compareImgIndex;
//  ui->imageView->updateImage();
//}
//
//void MainWindow::on_compareButton_released() {
//  ui->imageView->viewSettings.activeImgIndex = oldShowIdx;
//  ui->imageView->updateImage();
//}
//
//// void MainWindow::on_sampleButton_pressed() {
////   ui->imageView->resample();
////   ui->sampleButton->setEnabled(false);
////   newImage(
////       QString("Resampled
////       %1").arg(ui->imageView->sampleSettings.sampleMethod));
//// }
//
//// void MainWindow::on_loadImageButton_pressed() {
////   if (!ui->imageView->loadFile(QFileDialog::getOpenFileName(
////           this, "Load Image", "../images/",
////           tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)")))) {
////     return;
////   }
////   ui->imageView->viewSettings.compareImgIndex = 0;
////   ui->imageView->viewSettings.activeImgIndex = 0;
////   const QImage& img = ui->imageView->getActiveDisplayImage();
////   ui->depthNumLabel->setText(QString("%1").arg(img.depth()));
////   ui->sizeNumLabel->setText(
////       QString("%1 x %2").arg(img.width()).arg(img.height()));
////   ui->greyNumLabel->setText(QString("%1").arg(1 << img.depth()));
////   ui->quantisizeButton->setEnabled(true);
////   ui->computeSDFButton->setEnabled(true);
////   ui->viewSettingsGroupBox->setEnabled(true);
////   ui->imgSettingsGroupBox->setEnabled(true);
////   ui->sampleSettingsGroupBox->setEnabled(false);
////   ui->imageInfoGroupBox->setEnabled(true);
////   ui->imageView->scaleImToFit();
////   ui->savedImagesView->clear();
////   ui->compareImagesView->clear();
////   newImage("Original");
//// }
//
//// void MainWindow::on_quantisizeLevelSpinBox_valueChanged(int value) {
////   nitro::SampleSettings& sampleSettings = ui->imageView->sampleSettings;
////   sampleSettings.quantisizeLevel = value;
////   ui->quantisizeButton->setEnabled(true);
//// }
//
//// void MainWindow::on_methodComboBox_currentIndexChanged(int index) {
////   nitro::SampleSettings& sampleSettings = ui->imageView->sampleSettings;
////   sampleSettings.sampleMethod = static_cast<nitro::SampleMethod>(index);
////   ui->sampleButton->setEnabled(true);
//// }
//
//void MainWindow::on_fitButton_clicked() { ui->imageView->scaleImToFit(); }
//
//void MainWindow::on_resetButton_clicked() { ui->imageView->resetImScale(); }
//
//// void MainWindow::on_quantisizeButton_clicked() {
////   ui->imageView->quantisize();
////   ui->imageView->updateImage();
////   ui->quantisizeButton->setEnabled(false);
//
////  newImage(QString("Quantisized %1")
////               .arg(ui->imageView->sampleSettings.quantisizeLevel));
////  ui->computeSDFButton->setEnabled(true);
////}
//
//// void MainWindow::on_computeSDFButton_pressed() {
////   ui->computeSDFButton->setDown(true);
////   ui->imageView->calcDistanceField();
////   ui->computeSDFButton->setDown(false);
////   ui->computeSDFButton->setEnabled(false);
////   ui->sampleSettingsGroupBox->setEnabled(true);
////   ui->sampleButton->setEnabled(true);
//// }
//
//// void MainWindow::on_savedImagesView_itemSelectionChanged() {
////   int maxIdx = ui->imageView->savedImages.size() - 1;
////   ui->imageView->viewSettings.activeImgIndex =
////       MIN(ui->savedImagesView->currentIndex().row(), maxIdx);
////   ui->imageView->updateImage();
//// }
//
//// void MainWindow::on_compareImagesView_itemSelectionChanged() {
////   int maxIdx = ui->imageView->savedImages.size() - 1;
////   ui->imageView->viewSettings.compareImgIndex =
////       MIN(ui->compareImagesView->currentIndex().row(), maxIdx);
////   ui->imageView->updateImage();
//// }
//
//// void MainWindow::on_compressButton_pressed() {
////   //  CbdImage compressImg = ui->imageView->compress();
////   newImage(QString("Compressed"));
//// }
//
//// void MainWindow::on_ditheringCheckBox_toggled(bool checked) {
////   nitro::SampleSettings& sampleSettings = ui->imageView->sampleSettings;
////   sampleSettings.dither = checked;
////   ui->quantisizeButton->setEnabled(true);
//// }
//
//// void MainWindow::on_quantMethodComboBox_currentIndexChanged(int index) {
////   nitro::SampleSettings& sampleSettings = ui->imageView->sampleSettings;
////   sampleSettings.quantMethod = static_cast<nitro::QuantisizeMethod>(index);
////   ui->quantisizeButton->setEnabled(true);
//// }
//
//void MainWindow::on_differencePushButton_pressed() {
//  const QImage& imgA = ui->imageView->getImageByIndex(
//      ui->imageView->viewSettings.activeImgIndex);
//  const QImage& imgB = ui->imageView->getImageByIndex(
//      ui->imageView->viewSettings.compareImgIndex);
//  ui->imageView->savedImages.append(nitro::flipErr(imgA, imgB));
//  newImage(QString("Difference"));
//}
