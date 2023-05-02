#include "imagedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QPainter>
#include "config.hpp"

nitro::ImageDataModel::ImageDataModel()
        : _displayWrapper(nullptr),
          _imgLabel(nullptr) {
    connect(&nitro::config::configManager, &nitro::config::ConfigManager::nodeImagesChanged, this, &ImageDataModel::onNodeImagesChanged);
}

void nitro::ImageDataModel::onNodeImagesChanged() {
    _imgLabel->setHidden(!nitro::config::nodeImages);
    _displayWrapper->adjustSize();
//    Q_EMIT nodeUpdated();
}

QPixmap createPixmapWithGrid(int width, int height, int gridSize) {
    QPixmap pixmap(width, height);
    QtNodes::NodeStyle style;
    pixmap.fill(QColor(55, 55, 55));
    QPainter painter(&pixmap);
    painter.setPen(QColor(66, 66, 66));
    for (int x = 0; x < width; x += gridSize) {
        painter.drawLine(x, 0, x, height);
    }
    for (int y = 0; y < height; y += gridSize) {
        painter.drawLine(0, y, width, y);
    }
    painter.setPen(QColor(128, 128, 128));
    painter.drawRect(0, 0, width - 1, height - 1);
    painter.end();
    return pixmap;
}

QWidget *nitro::ImageDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
        auto *layout = new QVBoxLayout(_displayWrapper);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignRight);


        _imgLabel = new QLabel();
        auto p = createPixmapWithGrid(_embedImgSize, _embedImgSize, _embedImgSize / 6);
        _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());

        _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        _imgLabel->setAlignment(Qt::AlignCenter);
        _imgLabel->setHidden(!nitro::config::nodeImages);

        QWidget *before = initBeforeWidget();
        QWidget *after = initAfterWidget();

        Qt::Alignment widgetAlign = Qt::AlignLeft;

        if (before) {
            layout->addWidget(before);
            before->setFixedWidth(_imgLabel->width());
            layout->setAlignment(before, widgetAlign);
        }

        layout->addWidget(_imgLabel);
        layout->setAlignment(_imgLabel, widgetAlign);

        if (after) {
            layout->addWidget(after);
            after->setFixedWidth(_imgLabel->width());
            layout->setAlignment(after, widgetAlign);
        }
    }
    return _displayWrapper;
}

void nitro::ImageDataModel::updateImage(const QPixmap &p) {
    if (p.isNull()) {
        return;
    }
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
}

void nitro::ImageDataModel::updateImage(const QImage &img) {
    updateImage(QPixmap::fromImage(img));
}

void nitro::ImageDataModel::clearImage() {

    // TODO: construct this thing only once (statically)?
    auto p = createPixmapWithGrid(_embedImgSize, _embedImgSize, _embedImgSize / 6);
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());

    _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
    _imgLabel->setAlignment(Qt::AlignCenter);
}

