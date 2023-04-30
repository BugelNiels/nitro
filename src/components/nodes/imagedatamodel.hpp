#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QtNodes/NodeDelegateModel>

#include "cbdimage.hpp"
#include "src/components/nodes/imagedata.hpp"

namespace nitro {
    class ImageDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImageDataModel();

        virtual ~ImageDataModel() {}

    public:

        QWidget *embeddedWidget() override;

    protected:

        virtual QWidget *initBeforeWidget() = 0;

        virtual QWidget *initAfterWidget() = 0;

        void updateImage(const QPixmap& p);

    private:
        const int _embedImgSize = 128;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;

    };
}