#include "blenddatamodel.hpp"
#include "invaliddata.hpp"
#include "src/improc/core/algorithms/util/imgmath.hpp"

nitro::BlendDataModel::BlendDataModel() : ImageDataModel() {

}

QJsonObject nitro::BlendDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["blendVal"] = blendSlider->value();

    return modelJson;
}

void nitro::BlendDataModel::load(const QJsonObject &p) {
    QJsonValue jVal = p["blendVal"];
    if (!jVal.isUndefined()) {
        blendSlider->setValue(jVal.toInt());
        compute();
    }
}

QWidget *nitro::BlendDataModel::initBeforeWidget() {
    blendSlider = new QSlider(Qt::Horizontal);
    blendSlider->setMinimum(0);
    blendSlider->setMaximum(100);
    connect(blendSlider, &QSlider::valueChanged, this, [this] {
        compute();
    });
    return blendSlider;
}

void nitro::BlendDataModel::compute() {
    if (_inputImgA == nullptr || _inputImgB == nullptr) {
        return;
    }
    auto img = nitro::operations::mixImage(_inputImgA->getDisplayImg(), _inputImgB->getDisplayImg(),
                                           blendSlider->value() / 100.0f);
    auto ptrRes = std::make_shared<QImage>(img);
    _result = std::make_shared<nitro::ImageData>(ptrRes);
    if (_result != nullptr) {
        updateImage(_result->getDisplayImg());
    }
    Q_EMIT dataUpdated(0);
}


unsigned int nitro::BlendDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 2;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::BlendDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index < 2) {
                return ImageData().type();
            }
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return ImageData().type();
            }

        default:
            break;
    }
    return nitro::InvalidData().type();
}


void nitro::BlendDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || inputImg == nullptr) {
        clearImage();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    // TODO: check that dimensions agree and otherwise invalidate and emit warning
    if (portIndex == 0) {
        _inputImgA = inputImg;
    }
    if (portIndex == 1) {
        _inputImgB = inputImg;
    }
    compute();
}


std::shared_ptr<QtNodes::NodeData> nitro::BlendDataModel::outData(QtNodes::PortIndex port) {
    return _result;
}
