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
    setScene(imScene);
}

void nitro::ImageViewer::drawBackground(QPainter *painter, const QRectF &r) {
    QGraphicsView::drawBackground(painter, r);
    if (_oldImg != nullptr) {
        return;
    }

    auto drawGrid = [&](double gridStep) {
        QPointF tl(-emptySize, -emptySize);
        QPointF br(emptySize, emptySize);

        double left = std::floor(tl.x() / gridStep - 0.5);
        double right = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top = std::floor(br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi) {
            QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi) {
            QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
            painter->drawLine(line);
        }
    };

    // TODO: put in stylesheet somewhere
    QPen pfine(QColor(57, 62, 70), 1.0);

    painter->setPen(pfine);
    drawGrid(15);
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
                QMessageBox::information(this, tr("Save Successful"), QString("File saved to\n %1").arg(filePath));
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
    displayImg = new QImage(img);
    auto *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    if (_oldImg != nullptr) {
        scene()->removeItem(_oldImg);
    }
    scene()->addItem(item);


    // set correct scale
    item->setOffset(-item->boundingRect().width() / 2, -item->boundingRect().height() / 2);
    auto maxSize = std::max(img.width(), img.height());
    auto minCurSize = std::min(rect().width(), rect().height());
    double scale = minCurSize / double(maxSize);
    _oldImg = item;
    setupScale(0.8 * scale);
    repaint();
}


void nitro::ImageViewer::removeImage() {
    scene()->removeItem(_oldImg);
    _oldImg = nullptr;
    displayImg = nullptr;
    repaint();
}

void nitro::ImageViewer::centerScene() {
    if (scene()) {
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
    if (_oldImg == nullptr) {
        setupScale(1.0);
    } else {
        auto maxSize = std::max(_oldImg->boundingRect().width(), _oldImg->boundingRect().height());
        auto minCurSize = std::min(rect().width(), rect().height());
        double scale = minCurSize / maxSize;
        setupScale(0.8 * scale);
    }
}
