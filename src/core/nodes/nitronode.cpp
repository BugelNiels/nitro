#include <nodes/nitronode.hpp>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QImageReader>
#include <QLabel>
#include <QPushButton>
#include <QtNodes/InvalidData.hpp>
#include <doubleslider.hpp>
#include <intslider.hpp>
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/datatypes/integerdata.hpp>
#include <opencv2/imgcodecs.hpp>
#include <utility>

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

namespace nitro {

#define LABEL_SUFFIX "_1"
#define DEFAULT_BUTTON_TEXT "Open"

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

QtNodes::NodeDataType NitroNode::dataType(QtNodes::PortType portType,
                                          QtNodes::PortIndex portIndex) const {
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
    if (data == nullptr && sliderInputDeleted_) {
        // Give the input back to the slider widgets after the connection was deleted
        if (nodePorts_.inDataType(portIndex).id == DecimalData().type().id) {
            QString key = getInPortKey(portIndex);
            auto const *slider = dynamic_cast<DoubleSlider *>(widgets_[key]);
            data = std::make_shared<DecimalData>(slider->getVal());
        } else if (nodePorts_.inDataType(portIndex).id == IntegerData().type().id) {
            QString key = getInPortKey(portIndex);
            auto const *slider = dynamic_cast<IntSlider *>(widgets_[key]);
            data = std::make_shared<IntegerData>(slider->getVal());
        }
        sliderInputDeleted_ = false;
    } else if (data != nullptr) {
        // Verifies that integers and doubles don't go out of the bounds of the sliders
        // Probably a cleaner solution for this, but this will have to do for now
        if (data->type().id == DecimalData().type().id) {
            QString key = getInPortKey(portIndex);
            auto const *slider = dynamic_cast<DoubleSlider *>(widgets_[key]);
            if (slider) {
                double newVal = std::dynamic_pointer_cast<DecimalData>(data)->data();
                data = std::make_shared<DecimalData>(slider->boundVal(newVal));
            }
        } else if (data->type().id == IntegerData().type().id) {
            QString key = getInPortKey(portIndex);
            auto const *slider = dynamic_cast<IntSlider *>(widgets_[key]);
            if (slider) {
                int newVal = std::dynamic_pointer_cast<IntegerData>(data)->data();
                data = std::make_shared<IntegerData>(slider->boundVal(newVal));
            }
        }
    }
    // TODO: notify which part of the data changed
    nodePorts_.setInData(portIndex, data);
    if (algo_) {
        algo_->execute(nodePorts_);

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

void NitroNode::connectInputWidget(IntSlider *slider, QLabel *valLabel, int port) {
    QString key = getInPortKey(port);
    propJson_[key] = slider->getVal();
    widgets_[key] = slider;
    widgets_[key + LABEL_SUFFIX] = valLabel;
    widgetsJson_[key] = [slider](const QJsonValue &val) {
        slider->setVal(val.toInt());
        slider->setEnabled(true);
    };
    connect(slider, &IntSlider::valueUpdated, this, [this, key, port](int value) {
        setInData(std::make_shared<IntegerData>(value), port);
        propJson_[key] = value;
    });
}

void NitroNode::connectInputWidget(DoubleSlider *slider, QLabel *valLabel, int port) {
    QString key = getInPortKey(port);
    propJson_[key] = slider->getVal();
    widgets_[key] = slider;
    widgets_[key + LABEL_SUFFIX] = valLabel;
    widgetsJson_[key] = [slider](const QJsonValue &val) {
        slider->setVal(val.toDouble());
        slider->setEnabled(true);
    };
    connect(slider, &DoubleSlider::valueUpdated, this, [this, key, port](double value) {
        setInData(std::make_shared<DecimalData>(value), port);
        propJson_[key] = value;
    });
}

void NitroNode::connectLoadButton(const QString &name,
                                  QPushButton *button,
                                  int port,
                                  const QString &filter) {
    button->setText(DEFAULT_BUTTON_TEXT);
    QString key = getOutPortKey(port);
    widgets_[key] = button;
    widgetsJson_[key] = [this, button, name](const QJsonValue &val) {
        QString filePath = val.toString();
        nodePorts_.setGlobalProperty(name, filePath);
        algo_->execute(nodePorts_);

        if (filePath == "") {
            filePath = DEFAULT_BUTTON_TEXT;
        }
        QFontMetrics fontMetrics(button->font());
        QString elidedText = fontMetrics.elidedText(QFileInfo(filePath).fileName(),
                                                    Qt::ElideRight,
                                                    button->width() - 30);
        button->setText(elidedText);
        for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
            // Emit that everything has been updated
            Q_EMIT dataUpdated(i);
        }
    };
    connect(button, &QPushButton::pressed, this, [this, key, button, name, filter]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                        "NITRO File View",
                                                        "../data/",
                                                        filter);
        nodePorts_.setGlobalProperty(name, filePath);
        algo_->execute(nodePorts_);

        propJson_[key] = filePath;
        if (filePath == "") {
            filePath = DEFAULT_BUTTON_TEXT;
        }

        QFontMetrics fontMetrics(button->font());
        QString elidedText = fontMetrics.elidedText(QFileInfo(filePath).fileName(),
                                                    Qt::ElideRight,
                                                    button->width() - 30);
        button->setText(elidedText);
        for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
            // Emit that everything has been updated
            Q_EMIT dataUpdated(i);
        }
    });
}

void NitroNode::connectSourceInteger(IntSlider *slider, int port) {
    QString key = getOutPortKey(port);
    propJson_[key] = slider->getVal();
    widgets_[key] = slider;
    widgetsJson_[key] = [slider](const QJsonValue &val) {
        slider->setVal(val.toInt());
    };
    connect(slider, &IntSlider::valueUpdated, this, [this, key, port](int value) {
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
    connect(slider, &DoubleSlider::valueUpdated, this, [this, key, port](double value) {
        QString portName = nodePorts_.outPortName(port);
        nodePorts_.setOutputData(portName, std::make_shared<DecimalData>(value));
        propJson_[key] = value;
        Q_EMIT dataUpdated(port);
    });
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
    widgetsJson_[name] = [comboBox](const QJsonValue &val) {
        comboBox->setCurrentIndex(val.toInt());
    };
    connect(comboBox, &QComboBox::currentIndexChanged, this, [this, name](int idx) {
        nodePorts_.setOption(name, idx);
        propJson_[name] = idx;
        if (algo_) {
            algo_->execute(nodePorts_);
            for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
                Q_EMIT dataUpdated(i);
            }
        }
    });
}

void NitroNode::connectCheckBox(const QString &name, QCheckBox *checkBox) {
    propJson_[name] = checkBox->isChecked();
    widgetsJson_[name] = [checkBox](const QJsonValue &val) {
        checkBox->setChecked(val.toBool());
    };
    connect(checkBox, &QCheckBox::toggled, this, [this, name](bool checked) {
        nodePorts_.setOption(name, checked);
        propJson_[name] = checked;
        if (algo_) {
            algo_->execute(nodePorts_);
            for (int i = 0; i < nodePorts_.numOutPorts(); i++) {
                Q_EMIT dataUpdated(i);
            }
        }
    });
}

void NitroNode::connectLabel(const QString &name, QLabel *label) {
    propJson_[name] = label->text();
    widgetsJson_[name] = [label](const QJsonValue &val) {
        label->setText(val.toString());
    };
}

void NitroNode::inputConnectionCreated(const QtNodes::ConnectionId &connectionId) {
    QtNodes::PortIndex idx = connectionId.inPortIndex;
    QString key = getInPortKey(idx);
    if (widgets_[key] && nodePorts_.inDataType(idx).id == DecimalData().type().id ||
        nodePorts_.inDataType(idx).id == IntegerData().type().id) {
        widgets_[key]->setHidden(true);
        widgets_[key + LABEL_SUFFIX]->setHidden(false);
    }
}

void NitroNode::inputConnectionDeleted(const QtNodes::ConnectionId &connectionId) {
    QtNodes::PortIndex idx = connectionId.inPortIndex;

    QString key = getInPortKey(idx);
    if (widgets_[key] && nodePorts_.inDataType(idx).id == DecimalData().type().id ||
        nodePorts_.inDataType(idx).id == IntegerData().type().id) {
        widgets_[key]->setHidden(false);
        widgets_[key + LABEL_SUFFIX]->setHidden(true);
        sliderInputDeleted_ = true;
    }
}

} // namespace nitro
