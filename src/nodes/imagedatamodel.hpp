#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include "cbdimage.hpp"
#include "imagedata.hpp"

namespace nitro {
    class ImageDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImageDataModel();

        ~ImageDataModel() override = default;

    public:

        QWidget *embeddedWidget() override;

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

    };
}