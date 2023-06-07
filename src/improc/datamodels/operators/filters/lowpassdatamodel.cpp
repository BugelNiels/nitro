#include "lowpassdatamodel.hpp"

#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"
#include "core/algorithms/conversions/ycbcrconvert.hpp"
#include "core/algorithms/corrections.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>

#include "core/algorithms/filters/lowpassfilter.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/integerdata.hpp"

nitro::LowPassFilterDataModel::LowPassFilterDataModel() = default;

unsigned int nitro::LowPassFilterDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 3;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType
nitro::LowPassFilterDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index == 0) {
                return ImageData().type();
            }
            if (index == 1) {
                return DecimalData().type();
            }
            if (index == 2) {
                return IntegerData().type();
            }
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return ImageData().type();
            }

        default:
            break;
    }
    return QtNodes::InvalidData().type();
}

QWidget *nitro::LowPassFilterDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *horLayout = new QGridLayout();

    int rowIdx = 0;
    cutOffLabel = new QLabel("Cut-off");
    horLayout->addWidget(cutOffLabel, rowIdx, 0);
    cutoffSpinBox = new QDoubleSpinBox();
    cutoffSpinBox->setValue(cutoff);
    connect(cutoffSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        cutoff = value;
        recompute();
    });
    horLayout->addWidget(cutoffSpinBox, rowIdx, 1);
    rowIdx++;

    orderLabel = new QLabel("Order");
    horLayout->addWidget(orderLabel, rowIdx, 0);
    orderSpinBox = new QSpinBox();
    orderSpinBox->setValue(order);
    connect(orderSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        order = value;
        recompute();
    });
    horLayout->addWidget(orderSpinBox, rowIdx, 1);

    wrapper->setLayout(horLayout);
    return wrapper;
}


void nitro::LowPassFilterDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {

    if (portIndex == 0) {
        auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

        if (!data || !inputImg || !inputImg->isValid()) {
            clearImage();
            _result = nullptr;
            Q_EMIT dataUpdated(0);
            return;
        }
        if (inputImg->isColImg()) {
            auto channels = nitro::operations::separateYCbCr(*inputImg->colImage());
            auto yChannel = CbdImage(channels[0]);
            _input = std::make_shared<ImageData>(std::make_shared<CbdImage>(yChannel));
        } else {
            _input = inputImg;
        }
    }

    if (portIndex == 1) {
        auto inputVal = std::dynamic_pointer_cast<DecimalData>(data);
        cutoffSpinBox->setDisabled(inputVal != nullptr);
        if(inputVal != nullptr) {
            cutoffSpinBox->setValue(inputVal->value());
        }
    }
    if (portIndex == 2) {
        auto inputVal = std::dynamic_pointer_cast<IntegerData>(data);
        orderSpinBox->setDisabled(inputVal != nullptr);
        if(inputVal != nullptr) {
            orderSpinBox->setValue(inputVal->value());
        }
    }
    if (_input != nullptr && _input->isValid() ) {
        _result = filter(*_input->image());
        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}

void nitro::LowPassFilterDataModel::recompute() {

    if (_input == nullptr) {
        return;
    }
    if (_input->isValid()) {
        _result = filter(*_input->image());
        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::LowPassFilterDataModel::outData(QtNodes::PortIndex) {
    return _result;
}

QJsonObject nitro::LowPassFilterDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();
    modelJson["cutoff"] = cutoff;
    modelJson["order"] = order;

    return modelJson;
}

void nitro::LowPassFilterDataModel::load(const QJsonObject &p) {
    QJsonValue jCutoff = p["cutoff"];
    if (!jCutoff.isUndefined()) {
        cutoff = jCutoff.toDouble();
        if (cutoffSpinBox) {
            cutoffSpinBox->setValue(cutoff);
        }
    }

    QJsonValue jOrder = p["order"];
    if (!jOrder.isUndefined()) {
        order = jOrder.toInt();
        if (orderSpinBox) {
            orderSpinBox->setValue(order);
        }
    }
}


std::shared_ptr<nitro::ImageData>
nitro::LowPassFilterDataModel::filter(nitro::CbdImage &image) {
    auto result =  nitro::operations::lowPassFilter(image, cutoff, order);
    return std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(result));
}


