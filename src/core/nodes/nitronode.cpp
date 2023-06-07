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
        // TODO: extract into function
        QString key = QString("In %1").arg(portIndex);
        if(widgets_.count(key) > 0) {
            widgets_[key]->setEnabled(data == nullptr);
        }
        nodePorts_.setInData(portIndex, data);
        algo_->execute(nodePorts_, options_);

        for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
            // Emit that everything has been updated
            Q_EMIT dataUpdated(i);
        }
    }

    QWidget *NitroNode::embeddedWidget() {
        return widget_;
    }

    const QtNodes::NodeInfo &NitroNode::getInfo() const {
        return info_;
    }

    void NitroNode::connectInputWidget(QSpinBox *spinBox, int port) {
        QString key = QString("In %1").arg(port);
        widgets_[key] = spinBox;
        propJson_[key] = spinBox->value();
        widgetsJson_[key] = [spinBox](const QJsonValue &val) {
            spinBox->setValue(val.toInt());
        };
        connect(spinBox, &QSpinBox::valueChanged, this, [this, key, port](int value) {
            setInData(std::make_shared<IntegerData>(value), port);
            propJson_[key] = value;
        });
    }

    void NitroNode::connectInputWidget(QDoubleSpinBox *spinBox, int port) {
        QString key = QString("In %1").arg(port);
        propJson_[key] = spinBox->value();
        widgets_[key] = spinBox;
        widgetsJson_[key] = [spinBox](const QJsonValue &val) {
            spinBox->setValue(val.toDouble());
        };
        connect(spinBox, &QDoubleSpinBox::valueChanged, this, [this, key, port](double value) {
            setInData(std::make_shared<DecimalData>(value), port);
            propJson_[key] = value;
        });
    }

    void NitroNode::connectLoadButton(QPushButton *button, int port) {
        QString key = QString("Load %1").arg(port);
        widgets_[key] = button;
        widgetsJson_[key] = [this, button, port](const QJsonValue &val) {
            loadImage(button, port, val.toString());
        };
        connect(button, &QPushButton::pressed, this, [this, port, button]() {
            QString filePath = QFileDialog::getOpenFileName(
                    nullptr, "Load Image", "../images/",
                    tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

            loadImage(button, port, filePath);

        });
    }

    void NitroNode::connectSourceInteger(QSpinBox *spinBox, int port) {
        QString key = QString("Out %1").arg(port);
        propJson_[key] = spinBox->value();
        widgets_[key] = spinBox;
        widgetsJson_[key] = [spinBox](const QJsonValue &val) {
            spinBox->setValue(val.toInt());
        };
        connect(spinBox, &QSpinBox::valueChanged, this, [this, key, port](int value) {
            QString portName = nodePorts_.outPortName(port);
            nodePorts_.setOutputData(portName, std::make_shared<IntegerData>(value));
            propJson_[key] = value;
            Q_EMIT dataUpdated(port);
        });
    }

    void NitroNode::connectSourceValue(QDoubleSpinBox *spinBox, int port) {
        QString key = QString("Out %1").arg(port);
        propJson_[key] = spinBox->value();
        widgets_[key] = spinBox;
        widgetsJson_[key] = [spinBox](const QJsonValue &val) {
            spinBox->setValue(val.toDouble());
        };
        connect(spinBox, &QDoubleSpinBox::valueChanged, this, [this, key, port](double value) {
            QString portName = nodePorts_.outPortName(port);
            nodePorts_.setOutputData(portName, std::make_shared<DecimalData>(value));
            propJson_[key] = value;
            Q_EMIT dataUpdated(port);
        });
    }

    void NitroNode::loadImage(QPushButton *button, int port, const QString &filePath) {
        QImageReader reader(filePath);
        reader.setAutoTransform(true);
        QImage img = reader.read();

        QString portName = nodePorts_.outPortName(port);
        QString key = QString("Load %1").arg(port);

        if (img.isNull()) {
            button->setText("Load Image");
            nodePorts_.setOutputData(portName, nullptr);
            propJson_[key] = "";
        } else {
            button->setText(QFileInfo(filePath).fileName());
            propJson_[key] = filePath;
            if (img.isGrayscale()) {
                auto cbdImg = std::make_shared<CbdImage>(img);
                nodePorts_.setOutputType(port, GreyImageData().type());
                nodePorts_.setOutputData(portName, std::make_shared<GreyImageData>(cbdImg));
            } else {
                auto ptrImg = std::make_shared<QImage>(img);
                nodePorts_.setOutputType(port, ColorImageData().type());
                nodePorts_.setOutputData(portName, std::make_shared<ColorImageData>(ptrImg));
            }
        }
        Q_EMIT dataUpdated(port);
    }

    void NitroNode::load(const QJsonObject &loadJ) {
        propJson_ = loadJ["properties"].toObject();
        for (auto &key: propJson_.keys()) {
            widgetsJson_[key](propJson_[key]);
        }

    }

    QJsonObject NitroNode::save() const {
        QJsonObject modelJson = NodeDelegateModel::save();
        modelJson["properties"] = propJson_;
        return modelJson;
    }

    void NitroNode::connectComboBox(const QString &name, QComboBox *comboBox) {
        propJson_[name] = comboBox->currentIndex();
        options_[name] = comboBox->currentIndex();
        widgetsJson_[name] = [comboBox](const QJsonValue &val) {
            comboBox->setCurrentIndex(val.toInt());
        };
        connect(comboBox, &QComboBox::currentIndexChanged, this, [this, name](int idx) {
            options_[name] = idx;
            propJson_[name] = idx;
            algo_->execute(nodePorts_, options_);

            for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
                // For now just emit that everything has been updated
                Q_EMIT dataUpdated(i);
            }
        });

    }

} // nitro