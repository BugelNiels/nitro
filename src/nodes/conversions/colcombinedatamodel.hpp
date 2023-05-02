#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>


#include "cbdimage.hpp"
#include "src/nodes/imagedata.hpp"
#include "src/nodes/imagedatamodel.hpp"

namespace nitro {
    class ColorCombineDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        explicit ColorCombineDataModel(int numChannels);

        ~ColorCombineDataModel() override = default;

    public:
        bool captionVisible() const override { return true; }

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;


        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/combine.png"); }

        static QColor nodeColor() { return {59, 90, 115}; }

    protected:

        virtual void combine(const QVector<std::shared_ptr<ImageData>> &input) = 0;

        std::shared_ptr<ImageData> _result;

    private:
        QVector<std::shared_ptr<ImageData>> _input;


    };
}