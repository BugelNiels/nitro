#include "imageview.hpp"

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

#include "quantisizers/quantisizemethod.hpp"
#include "quantisizers/quantisizer.hpp"
#include "util/imgconvert.hpp"


nitro::ImageView::ImageView(QWidget *parent)
        : QScrollArea(parent), imageLabel(new QLabel), quantisized(false) {
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setScaledContents(true);
    this->setWidget(imageLabel);
}

nitro::ImageView::~ImageView() = default;

bool nitro::ImageView::loadFile(const QString &fileName) {
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(
                this, QGuiApplication::applicationDisplayName(),
                tr("Cannot load %1: %2")
                        .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
    originalImg.clear();
    quantisizedImg.clear();
    quantisized = false;
    scale = 1.0;
    savedImages.clear();
    //  newImage = newImage.convertToFormat(QImage::Format_Grayscale8);
    if (newImage.format() == QImage::Format_Grayscale8 ||
        newImage.format() == QImage::Format_Grayscale16) {
        originalImg.append(CbdImage(newImage));
    } else if (newImage.format() == QImage::Format_RGB32) {
        // extract channels
        auto rgb = separateYCbCr(newImage);
        originalImg.append(CbdImage(rgb[0]));
        originalImg.append(CbdImage(rgb[1]));
        originalImg.append(CbdImage(rgb[2]));
    } else {
        qDebug() << "Unsupported format" << newImage.format();
        return false;
    }
    savedImages.append(newImage);
    setImage(newImage);
    return true;
}

void nitro::ImageView::calcDistanceField() {
    resetProgress();
    if (quantisized) {
        for (auto &img: quantisizedImg) {
            img.calcDistanceField();
            addProgress(100 / float(quantisizedImg.size()));
        }
    } else {
        for (auto &img: originalImg) {
            img.calcDistanceField();
            addProgress(100 / float(originalImg.size()));
        }
    }
    finalizeProgress();
}

void nitro::ImageView::resample() {
    resetProgress();
    QVector<QImage> results;
    if (quantisized) {
        for (auto &img: quantisizedImg) {
            CbdImage resampled = img.resample(
                    sampleSettings.sampleLevel, getSampler(sampleSettings.sampleMethod));
            results.append(resampled.getDisplayImg());
            addProgress(100 / float(quantisizedImg.size()));
        }
    } else {
        for (auto &img: originalImg) {
            CbdImage resampled = img.resample(
                    sampleSettings.sampleLevel, getSampler(sampleSettings.sampleMethod));
            results.append(resampled.getDisplayImg());
            addProgress(100 / float(originalImg.size()));
        }
    }
    QImage dispImg = results[0];
    if (results.size() == 3) {
        dispImg = combineYCbCr(results);
    }
    savedImages.append(dispImg);
    finalizeProgress();
}

void nitro::ImageView::quantisize() {
    resetProgress();
    int dynRange = 1 << sampleSettings.quantisizeLevel;

    auto quantisizer = getQuantisizer(sampleSettings.quantMethod);

    quantisizedImg.clear();
    for (auto &img: originalImg) {
        quantisizedImg.append(
                quantisizer->quantisize(img, dynRange, sampleSettings.dither));
        addProgress(100 / float(originalImg.size()));
    }
    quantisized = true;
    QImage dispImg = quantisizedImg[0].getDisplayImg();
    if (quantisizedImg.size() == 3) {
        QVector<QImage> dispImgs;
        for (auto &img: quantisizedImg) {
            dispImgs.append(img.getDisplayImg());
        }
        dispImg = combineRGB(dispImgs);
    }
    savedImages.append(dispImg);
    finalizeProgress();
}

// CbdImage ImageView::compress() {
//   resetProgress();
//   auto &input = quantisized ? quantisizedImg : originalImg;
//   QVector<CbdImage> compressed;
//   for (auto &img : input) {
//     CbdImage compressImg =
//         img.compress(sampleSettings.quantisizeLevel,
//                      getSampler(sampleSettings.sampleMethod), this);
//     compressed.append(compressImg);
//   }

//  //  savedImages.append(compressImg.getDisplayImg());
//  setProgress(50);
//  //  return compressImg;
//  //  CbdImage resampled = compressImg.resample(
//  //      sampleSettings.sampleLevel,
//  getSampler(sampleSettings.sampleMethod));
//  //  savedImages.append(resampled.getDisplayImg());
//  //  //  CbdImage decompressImg = compressImg.decompress(8);
//  //  //  savedImages.append(decompressImg.getDisplayImg());
//  //  finalizeProgress();
//}

// void ImageView::decompress(CbdImage &compressImg) {
//   CbdImage decompressed = compressImg.decompress(
//       sampleSettings.sampleLevel, getSampler(sampleSettings.sampleMethod));
//   savedImages.append(decompressed.getDisplayImg());
//   //  CbdImage decompressImg = compressImg.decompress(8);
//   //  savedImages.append(decompressImg.getDisplayImg());
//   finalizeProgress();
// }

void nitro::ImageView::updateImage() {
    QImage img = getActiveDisplayImage();
    const QPixmap &p = QPixmap::fromImage(img);
    int w = img.width() * scale + 0.5;
    int h = img.height() * scale + 0.5;
    imageLabel->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
    imageLabel->adjustSize();
}

const QImage &nitro::ImageView::getActiveDisplayImage() {
    return savedImages[viewSettings.activeImgIndex];
}

const QImage &nitro::ImageView::getImageByIndex(int idx) { return savedImages[idx]; }

void nitro::ImageView::setImage(const QImage &img) {
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
void nitro::ImageView::mouseMoveEvent(QMouseEvent *event) {
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

void nitro::ImageView::mouseReleaseEvent(QMouseEvent *event) {
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
void nitro::ImageView::mousePressEvent(QMouseEvent *event) { setFocus(); }

/**
 * @brief MainView::wheelEvent Event that is called when the user scrolls.
 * @param event The mouse event.
 */
void nitro::ImageView::wheelEvent(QWheelEvent *event) {
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
void nitro::ImageView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case 'R':
            resetImScale();
            break;
    }
}

void nitro::ImageView::scaleImToFit() {
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

void nitro::ImageView::resetImScale() {
    const QPixmap &p = QPixmap::fromImage(getActiveDisplayImage());
    imageLabel->setPixmap(p);
    imageLabel->adjustSize();
    scale = 1;
}
