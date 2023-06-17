#include "nodes/nitronode.hpp"

#include <utility>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QImageReader>
#include <QFileDialog>
#include "QtNodes/InvalidData.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgcodecs.hpp>
#include "external/qt-value-slider/include/doubleslider.hpp"
#include "external/qt-value-slider/include/intslider.hpp"
#include "util.hpp"

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

namespace nitro {

    NitroNode::~NitroNode() = default;

    void NitroNode::init(QtNodes::NodeInfo info,
                         const NodePorts &nodePorts,
                         std::unique_ptr<NodeOperator> algo,
                         QWidget *widget) {
        info_ = std::move(info);
        algo_ = std::move(algo);
        nodePorts_ = nodePorts;
        widget_ = widget;
        // Force auto generating nodes to update
        setInData(nullptr, QtNodes::InvalidPortIndex);
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
            return nodePorts_.inDataType(int(portIndex));
        }
        if (portType == QtNodes::PortType::Out) {
            return nodePorts_.outDataType(int(portIndex));
        }
        return QtNodes::InvalidData().type();
    }

    std::shared_ptr<QtNodes::NodeData> NitroNode::outData(QtNodes::PortIndex portIndex) {
        return nodePorts_.getOutData(portIndex);
    }

    QString NitroNode::getInPortKey(unsigned int portIndex) const {
        return QString("In %1").arg(portIndex);
    }

    QString NitroNode::getOutPortKey(unsigned int portIndex) const {
        return QString("Out %1").arg(portIndex);
    }


    void NitroNode::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
        QString key = getInPortKey(portIndex);
        if (widgets_.count(key) > 0) {
            widgets_[key]->setEnabled(data == nullptr);
            if (auto slider = dynamic_cast<DoubleSlider *>(widgets_[key])) {
                if (auto val = dynamic_cast<DecimalData *>(data.get())) {
                    slider->setVal(val->value());
                }
            }
            if (auto slider = dynamic_cast<IntSlider *>(widgets_[key])) {
                if (auto val = dynamic_cast<IntegerData *>(data.get())) {
                    slider->setVal(val->value());
                }
            }
        }
        // TODO: check if the data changed
        nodePorts_.setInData(portIndex, data);
        if (algo_) {

            algo_->execute(nodePorts_, options_);

            for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
                // Emit that everything has been updated
                Q_EMIT dataUpdated(i);
            }
        }
    }


    QWidget *NitroNode::embeddedWidget() {
        return widget_;
    }

    const QtNodes::NodeInfo &NitroNode::getInfo() const {
        return info_;
    }

    void NitroNode::connectInputWidget(IntSlider *slider, int port) {
        QString key = getInPortKey(port);
        propJson_[key] = slider->getVal();
        widgets_[key] = slider;
        widgetsJson_[key] = [slider](const QJsonValue &val) {
            slider->setVal(val.toInt());
            slider->setEnabled(true);
        };
        connect(slider, &IntSlider::valueChanged, this, [this, key, port](int value) {
            setInData(std::make_shared<IntegerData>(value), port);
            propJson_[key] = value;
        });
    }

    void NitroNode::connectInputWidget(DoubleSlider *slider, int port) {
        QString key = getInPortKey(port);
        propJson_[key] = slider->getVal();
        widgets_[key] = slider;
        widgetsJson_[key] = [slider](const QJsonValue &val) {
            slider->setVal(val.toDouble());
            slider->setEnabled(true);
        };
        connect(slider, &DoubleSlider::valueChanged, this, [this, key, port](double value) {
            double actualVal = value / 100.0f;
            setInData(std::make_shared<DecimalData>(actualVal), port);
            propJson_[key] = actualVal;
        });
    }

    void NitroNode::connectLoadButton(QPushButton *button, int port) {
        QString key = QString("Load %1").arg(port);
        widgets_[key] = button;
        widgetsJson_[key] = [this, button, port](const QJsonValue &val) {
            loadImage(button, port, val.toString());
        };
        // TODO: extract this into general load button; pass lambda as parameter
        connect(button, &QPushButton::pressed, this, [this, port, button]() {
            QString filePath = QFileDialog::getOpenFileName(
                    nullptr, "Load Image", "../data/",
                    tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

            loadImage(button, port, filePath);

        });
    }

    void NitroNode::connectSourceInteger(IntSlider *slider, int port) {
        QString key = getOutPortKey(port);
        propJson_[key] = slider->getVal();
        widgets_[key] = slider;
        widgetsJson_[key] = [slider](const QJsonValue &val) {
            slider->setVal(val.toInt());
        };
        connect(slider, &DoubleSlider::valueChanged, this, [this, key, port](int value) {
            QString portName = nodePorts_.outPortName(port);
            nodePorts_.setOutputData(portName, std::make_shared<IntegerData>(value));
            propJson_[key] = value;
            Q_EMIT dataUpdated(port);
        });
    }

    void NitroNode::connectSourceValue(DoubleSlider *slider, int port) {
        QString key = getOutPortKey(port);
        propJson_[key] = slider->getVal();
        widgets_[key] = slider;
        widgetsJson_[key] = [slider](const QJsonValue &val) {
            slider->setVal(val.toDouble());
        };
        connect(slider, &DoubleSlider::valueChanged, this, [this, key, port](double value) {
            double actualVal = value / 100.0f;
            QString portName = nodePorts_.outPortName(port);
            nodePorts_.setOutputData(portName, std::make_shared<DecimalData>(actualVal));
            propJson_[key] = actualVal;
            Q_EMIT dataUpdated(port);
        });
    }

    void NitroNode::loadImage(QPushButton *button, int port, const QString &filePath) {
        cv::Mat inputImg = cv::imread(filePath.toStdString());

        QString portName = nodePorts_.outPortName(port);
        QString key = QString("Load %1").arg(port);

        if (inputImg.empty()) {
            return;
        } else {

            if (inputImg.channels() > 1 && isGrayscale(inputImg)) {
                cv::Mat gray;
                cvtColor(inputImg, gray, cv::COLOR_RGB2GRAY);
                inputImg = gray;
            }
            cv::Mat floatImg;
            float max = nitro::getMaxValue(inputImg);
            inputImg.convertTo(floatImg, CV_32F, 1.0 / max);
            inputImg = floatImg;

            QFontMetrics fontMetrics(button->font());
            QString elidedText = fontMetrics.elidedText(QFileInfo(filePath).fileName(), Qt::ElideRight,
                                                        button->width());
            button->setText(elidedText);
            propJson_[key] = filePath;
            auto ptrImg = std::make_shared<cv::Mat>(inputImg);
            nodePorts_.setOutputType(port, ImageData().type());
            nodePorts_.setOutputData(portName, std::make_shared<ImageData>(ptrImg));
        }
        Q_EMIT dataUpdated(port);
    }

    void NitroNode::load(const QJsonObject &loadJ) {
        propJson_ = loadJ["properties"].toObject();
        for (auto const &key: propJson_.keys()) {
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
                Q_EMIT dataUpdated(i);
            }
        });
    }

    void NitroNode::connectCheckBox(const QString &name, QCheckBox *checkBox) {
        propJson_[name] = checkBox->isChecked();
        options_[name] = checkBox->isChecked();
        widgetsJson_[name] = [checkBox](const QJsonValue &val) {
            checkBox->setChecked(val.toBool());
        };
        connect(checkBox, &QCheckBox::toggled, this, [this, name](bool checked) {
            options_[name] = checked;
            propJson_[name] = checked;
            algo_->execute(nodePorts_, options_);
            for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
                Q_EMIT dataUpdated(i);
            }
        });
    }

    void NitroNode::connectLabel(const QString &name, QLabel *label) {
        propJson_[name] = label->text();
        widgetsJson_[name] = [label](const QJsonValue &val) {
            label->setText(val.toString());
        };
    }

} // nitro
