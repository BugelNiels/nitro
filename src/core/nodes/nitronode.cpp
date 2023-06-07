#include "nitronode.hpp"

#include <utility>
#include <QPushButton>
#include <QImageReader>
#include <QFileDialog>
#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"

namespace nitro {


    NitroNode::~NitroNode() {
    }

    void NitroNode::init(QtNodes::NodeInfo info,
                         const NodePorts &nodePorts,
                         std::shared_ptr<NodeOperator> algo,
                         QWidget *widget) {
        info_ = std::move(info);
        algo_ = std::move(algo);
        nodePorts_ = nodePorts;
        widget_ = widget;
    }

    QString NitroNode::caption() const {
        return info_.getNodeName();
    }

    bool NitroNode::captionVisible() const {
        return true;
    }

    QString NitroNode::name() const {
        return info_.getNodeId();
    }

    unsigned int NitroNode::nPorts(QtNodes::PortType portType) const {
        if (portType == QtNodes::PortType::In) {
            return nodePorts_.numInPorts();
        }
        if (portType == QtNodes::PortType::Out) {
            return nodePorts_.numOutPorts();
        }
        return 0;
    }

    QtNodes::NodeDataType NitroNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
        if (portType == QtNodes::PortType::In) {
            return nodePorts_.inDataType(portIndex);
        }
        if (portType == QtNodes::PortType::Out) {
            return nodePorts_.outDataType(portIndex);
        }
        return QtNodes::InvalidData().type();
    }

    std::shared_ptr<QtNodes::NodeData> NitroNode::outData(QtNodes::PortIndex portIndex) {
        return nodePorts_.getOutData(portIndex);
    }

    void NitroNode::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
        if (data == nullptr) {
            // Do something?
            // Set result to null?
        }
        nodePorts_.setInData(portIndex, data);
        std::map<QString, QString> options;
        algo_->execute(nodePorts_, options);

        for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
            // For now just emit that everything has been updated
            Q_EMIT dataUpdated(i);
        }
    }

    // TODO: save and load functions

    QWidget *NitroNode::embeddedWidget() {
        return widget_;
    }

    const QtNodes::NodeInfo &NitroNode::getInfo() const {
        return info_;
    }

    void NitroNode::connectInputWidget(QSpinBox *spinBox, int port) {
        connect(spinBox, &QSpinBox::valueChanged, this, [this, port](int value) {
            setInData(std::make_shared<IntegerData>(value), port);
        });
    }

    void NitroNode::connectInputWidget(QDoubleSpinBox *spinBox, int port) {
        connect(spinBox, &QDoubleSpinBox::valueChanged, this, [this, port](double value) {
            setInData(std::make_shared<DecimalData>(value), port);
        });
    }

    void NitroNode::connectLoadButton(QPushButton *button, int port) {
        connect(button, &QPushButton::pressed, this, [this, port, button]() {
            QString filePath = QFileDialog::getOpenFileName(
                    nullptr, "Load Image", "../images/",
                    tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

            QImageReader reader(filePath);
            reader.setAutoTransform(true);
            QImage img = reader.read();

            QString portName = nodePorts_.outPortName(port);

            if (img.isNull()) {
                button->setText("Load Image");
                nodePorts_.setOutputData(portName, nullptr);
            } else {
                button->setText(QFileInfo(filePath).fileName());
                if (img.isGrayscale()) {
                    auto cbdImg = std::make_shared<nitro::CbdImage>(img);
                    nodePorts_.setOutputType(port, GreyImageData().type());
                    nodePorts_.setOutputData(portName, std::make_shared<GreyImageData>(cbdImg));
                } else {
                    auto ptrImg = std::make_shared<QImage>(img);
                    nodePorts_.setOutputType(port, ColorImageData().type());
                    nodePorts_.setOutputData(portName, std::make_shared<ColorImageData>(ptrImg));
                }
            }

            Q_EMIT dataUpdated(port);
        });
    }

} // nitro