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

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

ImageView::ImageView(QWidget *parent)
    : QScrollArea(parent), imageLabel(new QLabel) {
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageLabel->setScaledContents(true);
  this->setWidget(imageLabel);
}

ImageView::~ImageView() {}

bool ImageView::loadFile(const QString &fileName) {
  quantisized = false;
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
  setImage(newImage);
  return true;
}

void ImageView::quantisize() {
  resetProgress();
  quantisizedImg = quantisize(originalImg, sampleSettings.quantisizeLevel);
  quantisized = true;

  // We first need to multiply this to get it to show properly
  int width = quantisizedImg.width();
  int height = quantisizedImg.height();
  float scaleFactor = 255.0f / sampleSettings.quantisizeLevel;
  qDebug() << scaleFactor;
  displayQuantImg = QImage(width, height, QImage::Format_Grayscale8);

  for (int y = 0; y < height; y++) {
    const uchar *inputRow = quantisizedImg.constScanLine(y);
    uchar *upscaledRow = displayQuantImg.scanLine(y);
    for (int x = 0; x < width; x++) {
      upscaledRow[x] = inputRow[x] * scaleFactor;
    }
  }
  finalizeProgress();
}

void ImageView::calcDistanceField() {
  QImage &inputImg = quantisized ? quantisizedImg : originalImg;
  distanceField =
      calcDistanceField(inputImg, sampleSettings.use3dDistanceField);
}

void ImageView::resample() {
  QImage &inputImg = quantisized ? quantisizedImg : originalImg;
  resmapledImg =
      resample(inputImg, sampleSettings.sampleLevel, sampleSettings.resampler);
}

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

static float **allocFloatMatrix(int width, int height) {
  float **matrix = (float **)(malloc(height * sizeof(float *) +
                                     width * height * sizeof(float)));
  float *p = (float *)(matrix + height);
  for (int y = 0; y < height; y++) {
    matrix[y] = p + width * y;
  }
  return matrix;
}

static float computeEuclideanDistance(int xp, int yp, QImage &image, int col,
                                      int d) {
  int width = image.width();
  int height = image.height();
  int minDistance = width * width + height * height;
  for (int y = yp; y < height; y++) {
    int yDiff2 = (yp - y) * (yp - y);
    uchar *inputRow = image.scanLine(y);
    for (int x = xp; x < width; x++) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }
    for (int x = xp - 1; x >= 0; x--) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }

    if (yDiff2 > minDistance) {
      break;
    }
  }

  for (int y = yp - 1; y >= 0; y--) {
    int yDiff2 = (yp - y) * (yp - y);
    uchar *inputRow = image.scanLine(y);
    for (int x = xp; x < width; x++) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }
    for (int x = xp - 1; x >= 0; x--) {
      int pixel = inputRow[x];
      if ((pixel < d && col >= d) || (pixel >= d && col < d)) {
        int xDiff2 = (xp - x) * (xp - x);
        int distance = xDiff2 + yDiff2;
        if (distance > minDistance) {
          break;
        } else {
          minDistance = distance;
        }
      }
    }

    if (yDiff2 > minDistance) {
      break;
    }
  }

  return col >= d ? -std::sqrt(minDistance) : std::sqrt(minDistance);
}

static float **getDistanceField(QImage &image, int d) {
  int width = image.width();
  int height = image.height();
  float **output = allocFloatMatrix(width, height);

#pragma omp parallel for
  for (int y = 0; y < height; y++) {
    uchar *inputRow = image.scanLine(y);
    for (int x = 0; x < width; x++) {
      output[y][x] = computeEuclideanDistance(x, y, image, inputRow[x], d);
    }
  }
  return output;
}

QVector<float **> ImageView::calcDistanceField(QImage &image, bool use3D) {
  resetProgress();
  int width = image.width();
  int height = image.height();
  int depth = image.depth();
  int dynRange = quantisized ? sampleSettings.quantisizeLevel : 1 << depth;
  QVector<float **> df;
  df.resize(dynRange);

  float **l0 = allocFloatMatrix(width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      l0[y][x] = -10;
    }
  }

  df[0] = l0;
  setProgress(1.0f / static_cast<float>(dynRange) * 100);

  for (int d = 1; d < dynRange; d++) {
    df[d] = getDistanceField(image, d);
    setProgress((d + 1) / static_cast<float>(dynRange) * 100);
  }
  finalizeProgress();
  return df;
}

QImage ImageView::resample(QImage &image, int numLevels, Resampler *resampler) {
  int depth = image.depth();
  int dynRange = quantisized ? sampleSettings.quantisizeLevel : 1 << depth;
  dynRange--;
  return resampler->resample(image, dynRange, distanceField, numLevels);
}

void ImageView::updateImage() {
  imageLabel->setPixmap(QPixmap::fromImage(getActiveImage(true)));
}

const QImage &ImageView::getActiveImage(bool display) {
  switch (viewSettings.activeImage) {
    case ORIGINAL:
      return originalImg;
    case QUANTISIZED:
      return display ? displayQuantImg : quantisizedImg;
    case RESAMPLED:
      return resmapledImg;
  }
}

void ImageView::setImage(const QImage &image) {
  imageLabel->setPixmap(QPixmap::fromImage(image));
  scale = 1.0;
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
  const QImage &img = getActiveImage(true);
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
  const QImage &img = getActiveImage(true);
  int w = width();
  int h = height();
  const QPixmap &p = QPixmap::fromImage(img);
  // calculate new scale factor
  imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
  int newW = imageLabel->pixmap().width();
  scale = newW / static_cast<float>(img.width());
  imageLabel->adjustSize();
}
void ImageView::resetImScale() {
  const QPixmap &p = QPixmap::fromImage(getActiveImage(true));
  imageLabel->setPixmap(p);
  imageLabel->adjustSize();
  scale = 1;
}
