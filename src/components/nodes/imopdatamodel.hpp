#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>


#include "cbdimage.hpp"
#include "imagedata.hpp"

namespace nitro {
    class ImOpDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImOpDataModel();

        virtual ~ImOpDataModel() {}

    public:
        bool captionVisible() const override { return true; }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

        QWidget *embeddedWidget() override;

    protected:
        virtual nitro::CbdImage compute(const nitro::CbdImage &inputImg) = 0;

    private:

        const int _embedImgSize = 256;
        std::shared_ptr<ImageData> _result;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;
    };
}