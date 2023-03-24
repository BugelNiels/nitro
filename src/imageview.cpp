#include "imageview.h"

#include <math.h>
#include <omp.h>

#include <QColorSpace>
#include <QDir>
#include <QGuiApplication>
#include <QImage>
#include <QImageReader>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QtAlgorithms>

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

ImageView::ImageView(QWidget *parent)
    : QScrollArea(parent), imageLabel(new QLabel), quantisized(false) {
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageLabel->setScaledContents(true);
  this->setWidget(imageLabel);
}

ImageView::~ImageView() {}

bool ImageView::loadFile(const QString &fileName) {
  quantisized = false;
  scale = 1.0;
  QImageReader reader(fileName);
  reader.setAutoTransform(true);
  const QImage newImage = reader.read();
  if (newImage.isNull()) {
    QMessageBox::information(
        this, QGuiApplication::applicationDisplayName(),
        tr("Cannot load %1: %2")
            .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
    return false;
  }
  originalImg = newImage;
  savedImages.clear();
  savedImages.append(originalImg);
  setImage(originalImg);
  return true;
}

void ImageView::calcDistanceField() {
  QImage &inputImg = quantisized ? quantisizedImg : originalImg;
  if (sampleSettings.use3dDistanceField) {
    distanceField = calcDistanceField(inputImg, sampleSettings.distMult);
  } else {
    distanceField = calcDistanceField(inputImg);
  }
}

void ImageView::resample() {
  QImage &inputImg = quantisized ? quantisizedImg : originalImg;
  savedImages.append(resample(inputImg, sampleSettings.sampleLevel,
                              getSampler(sampleSettings.sampleMethod)));
}

// void medianFilter(QImage &image, int filterSize) {
//   int width = image.width();
//   int height = image.height();
//   QVector<int> vals;
//   vals.reserve(filterSize * filterSize);

//  QImage result(width, height, QImage::Format_Grayscale8);
//  int a = filterSize / 2;
//  for (int y = 0; y < height; y++) {
//    uchar *inputRow = image.scanLine(y);
//    uchar *outputRow = result.scanLine(y);
//    for (int x = 0; x < width; x++) {
//      vals.clear();

//      for (int yf = -a; yf < a; yf++) {
//        int yi = y + yf;
//        if (yi < 0 || yi >= height) {
//          break;
//        }
//        for (int xf = -a; xf < a; xf++) {
//          int xi = x + xf;
//          if (xi < 0 || xi >= width) {
//            break;
//          }
//          vals.append(image.scanLine(yi)[xi]);
//        }
//      }
//      std::sort(vals);
//      int val = inputRow[x];

//      float proportion = val / (float)(dynRange);
//      int newVal = proportion * (newNumGreyLevels - 1) + 0.5f;
//      resampledRow[x] = newVal;
//    }
//    setProgress(y / (float)height * 90);
//  }
//}

QImage ImageView::quantisize(QImage &image, int newNumGreyLevels) {
  int width = image.width();
  int height = image.height();
  int depth = image.depth();
  int dynRange = 1 << depth;
  QImage quantisizedImg(width, height, QImage::Format_Grayscale8);
  quantisizedImg.fill(0);

  for (int y = 0; y < height; y++) {
    uchar *inputRow = image.scanLine(y);
    uchar *resampledRow = quantisizedImg.scanLine(y);
    for (int x = 0; x < width; x++) {
      int val = inputRow[x];

      float proportion = val / (float)(dynRange);
      int newVal = proportion * (newNumGreyLevels - 1) + 0.5f;
      resampledRow[x] = newVal;
    }
    setProgress(y / (float)height * 90);
  }
  return quantisizedImg;
}

QImage ImageView::resample(QImage &image, int numDesiredLevels,
                           std::shared_ptr<Resampler> resampler) {
  return resampler->resample(image, distanceField, numDesiredLevels);
}

void ImageView::updateImage() {
  QImage img = getActiveDisplayImage();
  const QPixmap &p = QPixmap::fromImage(img);
  int w = img.width() * scale + 0.5;
  int h = img.height() * scale + 0.5;
  imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
  imageLabel->adjustSize();
}

const QImage &ImageView::getActiveDisplayImage() {
  return savedImages[viewSettings.activeImgIndex];
}

void ImageView::setImage(const QImage &img) {
  const QPixmap &p = QPixmap::fromImage(img);
  int w = img.width() * scale + 0.5;
  int h = img.height() * scale + 0.5;
  imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
  imageLabel->adjustSize();
}

/**
 * @brief MeshView::mouseMoveEvent Event that is called when the mouse is moved.
 * @param Event The mouse event.
 */
void ImageView::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    QVector2D sPos(event->position().x(), event->position().y());

    const auto &hBar = horizontalScrollBar();
    const auto &vBar = verticalScrollBar();
    if (!dragging) {
      dragging = true;
      oldMouseCoords = sPos;
      oldScrollValue = QVector2D(hBar->value(), vBar->value());
      return;
    }

    QVector2D change = sPos - oldMouseCoords;

    float changeX = change.x() / width() * (hBar->maximum() - hBar->minimum());
    float changeY = change.y() / height() * (vBar->maximum() - vBar->minimum());

    hBar->setValue(oldScrollValue.x() - changeX);
    vBar->setValue(oldScrollValue.y() - changeY);
    update();
  } else {
    dragging = false;
  }
}

void ImageView::mouseReleaseEvent(QMouseEvent *event) {
  QVector2D sPos(event->position().x(), event->position().y());

  const auto &hBar = horizontalScrollBar();
  const auto &vBar = verticalScrollBar();
  dragging = true;
  oldMouseCoords = sPos;
  oldScrollValue = QVector2D(hBar->value(), vBar->value());
  dragging = false;
}

/**
 * @brief RenderView::mousePressEvent Event that is called when the mouse is
 * pressed.
 * @param event The mouse event.
 */
void ImageView::mousePressEvent(QMouseEvent *event) { setFocus(); }

/**
 * @brief MainView::wheelEvent Event that is called when the user scrolls.
 * @param event The mouse event.
 */
void ImageView::wheelEvent(QWheelEvent *event) {
  float phi = 1.0f + (event->angleDelta().y() / 1000.0f);
  scale = fmin(fmax(phi * scale, 0.1f), 10.0f);
  const QImage &img = getActiveDisplayImage();
  int w = img.width() * scale + 0.5;
  int h = img.height() * scale + 0.5;

  // set scrollbar correctly
  const auto &hBar = horizontalScrollBar();
  const auto &vBar = verticalScrollBar();
  float scrollX =
      hBar->value() / static_cast<float>(hBar->maximum() - hBar->minimum());
  float scrollY =
      vBar->value() / static_cast<float>(vBar->maximum() - vBar->minimum());

  const QPixmap &p = QPixmap::fromImage(img);
  imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
  imageLabel->adjustSize();

  hBar->setValue(scrollX * (hBar->maximum() - hBar->minimum()));
  vBar->setValue(scrollY * (vBar->maximum() - vBar->minimum()));
}

/**
 * @brief MainView::keyPressEvent Event that is called when a key is pressed.
 * @param event The key event.
 */
void ImageView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case 'R':
      resetImScale();
      break;
  }
}

void ImageView::scaleImToFit() {
  const QImage &img = getActiveDisplayImage();
  const QPixmap &p = QPixmap::fromImage(img);
  int w = width();
  int h = height();
  // calculate new scale factor
  imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
  int newW = imageLabel->pixmap().width();
  scale = newW / static_cast<float>(img.width());
  imageLabel->adjustSize();
}

void ImageView::resetImScale() {
  const QPixmap &p = QPixmap::fromImage(getActiveDisplayImage());
  imageLabel->setPixmap(p);
  imageLabel->adjustSize();
  scale = 1;
}
