#include "imgviewer.hpp"
#include <QtNodes/internal/StyleCollection.hpp>

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
#include <cmath>
#include <QMenu>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QFileDialog>

nitro::ImageViewer::ImageViewer(ImageViewerScene *imScene, QWidget *parent)
        : QGraphicsView(parent) {


    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    setResizeAnchor(QGraphicsView::transformationAnchor());

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setCacheMode(QGraphicsView::CacheBackground);
    // TODO: put in stylesheet
    setBackgroundBrush(QColor(34, 40, 49));
    setAutoFillBackground(true);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    setScaleRange(0.3, 20);

    // Sets the scene rect to its maximum possible ranges to avoid auto scene range
    // re-calculation when expanding the all QGraphicsItems common rect.
    int maxSize = 4960;
    setSceneRect(-maxSize, -maxSize, (maxSize * 2), (maxSize * 2));
//    resetImScale();
    setScene(imScene);
}

void nitro::ImageViewer::drawBackground(QPainter *painter, const QRectF &r) {
    QGraphicsView::drawBackground(painter, r);
    painter->setBrush(QBrush(QColor(42, 42, 42)));
    painter->drawRect(r);
    if (_imgDisplayItem != nullptr) {
        return;
    }
    QPen pBounds(QColor(128, 128, 128), 2.0);
    painter->setPen(pBounds);
    QRectF gridRect(-emptySize, -emptySize, emptySize * 2, emptySize * 2);
    painter->drawRect(gridRect);

    QPen pfine(QColor(66, 66, 66), 1.0);

    const int gridSize = 32;
    painter->setPen(pfine);
    for (qreal x = gridRect.x(); x < gridRect.x() + gridRect.width(); x += gridSize) {
        painter->drawLine(x, gridRect.y(), x, gridRect.y() + gridRect.height());
    }
    for (qreal y = gridRect.y(); y < gridRect.y() + gridRect.height(); y += gridSize) {
        painter->drawLine(gridRect.x(), y, gridRect.x() + gridRect.width(), y);
    }
}

void nitro::ImageViewer::setScaleRange(double minimum, double maximum) {
    if (maximum < minimum)
        std::swap(minimum, maximum);
    minimum = std::max(0.0, minimum);
    maximum = std::max(0.0, maximum);

    _scaleRange = {minimum, maximum};

    setupScale(transform().m11());
}

void nitro::ImageViewer::scaleUp() {
    double const step = 1.2;
    double const factor = std::pow(step, 1.0);

    if (_scaleRange.maximum > 0) {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() >= _scaleRange.maximum) {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    Q_EMIT scaleChanged(transform().m11());
}

void nitro::ImageViewer::scaleDown() {
    double const step = 1.2;
    double const factor = std::pow(step, -1.0);

    if (_scaleRange.minimum > 0) {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() <= _scaleRange.minimum) {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    Q_EMIT scaleChanged(transform().m11());
}

void nitro::ImageViewer::setupScale(double scale) {
    scale = std::max(_scaleRange.minimum, std::min(_scaleRange.maximum, scale));

    if (scale <= 0)
        return;

    if (scale == transform().m11())
        return;

    QTransform matrix;
    matrix.scale(scale, scale);
    setTransform(matrix, false);

    Q_EMIT scaleChanged(scale);
}

nitro::ImageViewer::~ImageViewer() = default;

void nitro::ImageViewer::wheelEvent(QWheelEvent *event) {
    QPoint delta = event->angleDelta();

    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());

    if (d > 0.0)
        scaleUp();
    else
        scaleDown();
}

QMenu *nitro::ImageViewer::createContextMenu() {
    auto *menu = new QMenu();
    auto *resetAction = new QAction("Reset view", this);
    QObject::connect(resetAction, &QAction::triggered, [this]() {
        resetImScale();
    });
    menu->addAction(resetAction);

    auto *saveAction = new QAction("Save image", this);
    QObject::connect(saveAction, &QAction::triggered, [this]() {
        if (displayImg != nullptr) {
            QString filePath = QFileDialog::getSaveFileName(
                    this, "Save Image", "../images/",
                    tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));
            if (filePath == "") {
                return;
            }
            if (displayImg->save(filePath)) {
                QMessageBox::information(this, tr("Save Successful"),
                                         QString("File canQuitSafely to\n %1").arg(filePath));
            } else {
                QMessageBox::warning(this, tr("Could not save"),
                                     QString("Something went wrong while trying to save to\n %1").arg(filePath));
            }
        }
    });
    menu->addAction(saveAction);
    return menu;
}

void nitro::ImageViewer::contextMenuEvent(QContextMenuEvent *event) {
    QMenu *menu = createContextMenu();
    if (menu) {
        menu->exec(event->globalPos());
    }
}

void nitro::ImageViewer::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
}

void nitro::ImageViewer::setImage(const QImage &img) {
    if (img.sizeInBytes() == 0) {
        return;
    }
    if (_imgDisplayItem == nullptr) {
        _imgDisplayItem = new QGraphicsPixmapItem(QPixmap::fromImage(img));
        scene()->addItem(_imgDisplayItem);
        QRectF rect = scene()->itemsBoundingRect();
        scene()->setSceneRect(rect);
        resetImScale();
    } else {
        _imgDisplayItem->setPixmap(QPixmap::fromImage(img));
        if (_imgDisplayItem->boundingRect().width() != displayImg->width() ||
            _imgDisplayItem->boundingRect().height() != displayImg->height()) {
            QRectF rect = scene()->itemsBoundingRect();
            scene()->setSceneRect(rect);
            resetImScale();
        }
    }
    displayImg = new QImage(img);
    _replacementDue = false;
    repaint();
}


void nitro::ImageViewer::awaitReplacement() {
    _replacementDue = true;
}

void nitro::ImageViewer::removeImage() {
    if (_replacementDue) {
        return;
    }
    scene()->removeItem(_imgDisplayItem);
    _imgDisplayItem = nullptr;
    repaint();
}

void nitro::ImageViewer::centerScene() {
    if (scene()) {
        qDebug() << "centering";
        scene()->setSceneRect(QRectF());

        QRectF sceneRect = scene()->sceneRect();

        if (sceneRect.width() > this->rect().width() || sceneRect.height() > this->rect().height()) {
            fitInView(sceneRect, Qt::KeepAspectRatio);
        }

        centerOn(sceneRect.center());
    }
}

void nitro::ImageViewer::resetImScale() {
    centerScene();
    if (_imgDisplayItem == nullptr) {
        setupScale(1.0);
    } else {
        auto maxSize = std::max(_imgDisplayItem->boundingRect().width(), _imgDisplayItem->boundingRect().height());
        auto minCurSize = std::min(rect().width(), rect().height());
        double scale = minCurSize / maxSize;
        setupScale(0.8 * scale);
    }
}
