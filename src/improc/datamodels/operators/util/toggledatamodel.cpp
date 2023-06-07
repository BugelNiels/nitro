#include "toggledatamodel.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"

nitro::ToggleDataModel::ToggleDataModel() : ImageDataModel() {

}

QJsonObject nitro::ToggleDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["checked"] = toggleCheckBox->isChecked();

    return modelJson;
}

void nitro::ToggleDataModel::load(const QJsonObject &p) {
    QJsonValue jChecked = p["checked"];
    if (!jChecked.isUndefined()) {
        toggleCheckBox->setChecked(jChecked.toBool());
        compute();
    }
}

QWidget *nitro::ToggleDataModel::initBeforeWidget() {
    toggleCheckBox = new QCheckBox("Toggle");
    toggleCheckBox->setChecked(true);
    connect(toggleCheckBox, &QCheckBox::toggled, this, [this] {
        compute();
    });
    return toggleCheckBox;
}

void nitro::ToggleDataModel::compute() {
    if (toggleCheckBox->isChecked()) {
        _result = _inputImgA;
    } else {
        _result = _inputImgB;
    }
    if(_result != nullptr) {
        updateImage(_result->getDisplayImg());
    }Q_EMIT
    dataUpdated(0);
}


unsigned int nitro::ToggleDataModel::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType nitro::ToggleDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
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
    return QtNodes::InvalidData().type();
}


void nitro::ToggleDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
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


std::shared_ptr<QtNodes::NodeData> nitro::ToggleDataModel::outData(QtNodes::PortIndex port) {
    return _result;
}
