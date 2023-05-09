#include "mixdatamodel.hpp"
#include "invaliddata.hpp"
#include "src/improc/core/algorithms/util/imgmath.hpp"

nitro::MixDataModel::MixDataModel() : ImageDataModel() {
    supportedModes["Xor"] = QPainter::CompositionMode_Xor;
    supportedModes["Plus"] = QPainter::CompositionMode_Plus;
    supportedModes["Multiply"] = QPainter::CompositionMode_Multiply;

    supportedModes["Screen"] = QPainter::CompositionMode_Screen;
    supportedModes["Overlay"] = QPainter::CompositionMode_Overlay;
    supportedModes["Darken"] = QPainter::CompositionMode_Darken;
    supportedModes["Lighten"] = QPainter::CompositionMode_Lighten;

    supportedModes["Color Dodge"] = QPainter::CompositionMode_ColorDodge;
    supportedModes["Color Burn"] = QPainter::CompositionMode_ColorBurn;

    supportedModes["Hard Light"] = QPainter::CompositionMode_HardLight;
    supportedModes["Soft Light"] = QPainter::CompositionMode_SoftLight;

    supportedModes["Difference"] = QPainter::CompositionMode_Difference;
    supportedModes["Exclusion"] = QPainter::CompositionMode_Exclusion;
}

QJsonObject nitro::MixDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["mode"] = modeBox->currentText();

    return modelJson;
}

void nitro::MixDataModel::load(const QJsonObject &p) {
    QJsonValue jVal = p["blendVal"];
    if (!jVal.isUndefined()) {
        modeBox->setCurrentText(jVal.toString());
        compute();
    }
}

QWidget *nitro::MixDataModel::initBeforeWidget() {
    modeBox = new QComboBox;
    auto keys= supportedModes.keys();
    for(auto& k : keys) {
        modeBox->addItem(k);
    }
    connect(modeBox, &QComboBox::currentIndexChanged, this, [this] {
        compute();
    });
    return modeBox;
}

void nitro::MixDataModel::compute() {
    if (_inputImgA == nullptr || _inputImgB == nullptr) {
        return;
    }
    auto img = nitro::operations::mixImage(*_inputImgA, *_inputImgB, supportedModes[modeBox->currentText()]);
    auto ptrRes = std::make_shared<QImage>(img);
    _result = std::make_shared<nitro::ImageData>(ptrRes);
    if (_result != nullptr) {
        updateImage(_result->getDisplayImg());
    }
    Q_EMIT dataUpdated(0);
}


unsigned int nitro::MixDataModel::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType nitro::MixDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
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


void nitro::MixDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || inputImg == nullptr) {
        clearImage();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    // TODO: check that dimensions agree and otherwise invalidate and emit warning
    if (portIndex == 0 && inputImg->isValid()) {
        _inputImgA = std::make_shared<QImage>(inputImg->getDisplayImg());
    }
    if (portIndex == 1 && inputImg->isValid()) {
        _inputImgB = std::make_shared<QImage>(inputImg->getDisplayImg());
    }
    compute();
}


std::shared_ptr<QtNodes::NodeData> nitro::MixDataModel::outData(QtNodes::PortIndex port) {
    return _result;
}
