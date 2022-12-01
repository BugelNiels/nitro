#include "imageview.h"

#include <math.h>

#include <QColorSpace>
#include <QDir>
#include <QGuiApplication>
#include <QImage>
#include <QImageReader>
#include <QMessageBox>

ImageView::ImageView(QWidget *parent)
    : QScrollArea(parent), imageLabel(new QLabel) {
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);
  this->setWidget(imageLabel);
}

ImageView::~ImageView() {}

void ImageView::loadFile(const QString &fileName) {
  QImageReader reader(fileName);
  reader.setAutoTransform(true);
  const QImage newImage = reader.read();
  if (newImage.isNull()) {
    QMessageBox::information(
        this, QGuiApplication::applicationDisplayName(),
        tr("Cannot load %1: %2")
            .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
    return;
  }
  setImage(newImage);
}

void ImageView::setImage(const QImage &newImage) {
  image = newImage;
  if (image.colorSpace().isValid()) {
    image.convertToColorSpace(QColorSpace::SRgb);
  }
  imageLabel->setPixmap(QPixmap::fromImage(image));
  scaleFactor = 1.0;
  imageLabel->adjustSize();
}
