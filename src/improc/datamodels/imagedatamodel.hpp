#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QtNodes/NodeDelegateModel>

#include "src/improc/core/cbdimage.hpp"
#include "imagedata.hpp"

namespace nitro {
    class ImageDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImageDataModel();

        ~ImageDataModel() override = default;

    public:

        QWidget *embeddedWidget() override;

        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/node_var1.png"); }

    protected:

        virtual QWidget *initBeforeWidget() { return nullptr; }

        virtual QWidget *initAfterWidget() { return nullptr; }

        void updateImage(const QPixmap &p);

        void updateImage(const QImage &img);

        void clearImage();

    private:
        const int _embedImgSize = 128;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;

        void onNodeImagesChanged();
    };
}