#include <QVBoxLayout>
#include <QSpinBox>
#include <utility>
#include "nitronodebuilder.hpp"
#include "datamodels/datatypes/imagedata.hpp"
#include "datamodels/datatypes/integerdata.hpp"
#include "datamodels/datatypes/decimaldata.hpp"
#include "nodeeditor/style/nodecolors.hpp"

using namespace nitro;

NitroNodeBuilder::NitroNodeBuilder(const QString name, const QString id, const QString category, std::shared_ptr<NodeAlgorithm> algo)
        : name_(name),
          id_(id),
          category_(category),
          algo_(std::move(algo)) {

    _displayWrapper = new QWidget();
    _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
    auto *layout = new QVBoxLayout(_displayWrapper);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignRight);
}

std::unique_ptr<NitroNode> NitroNodeBuilder::build() {
    NodeInfo info(name_, id_, nodeColor_, iconPath_);

    // TODO: perhaps not the best way, but works for now
    nitro::NodeColors::registerColor(info);
    return std::make_unique<NitroNode>(info,
                                       inputList_,
                                       outputList_,
                                       inputMap_,
                                       outputMap_,
                                       algo_,
                                       _displayWrapper);
}

NitroNodeBuilder *NitroNodeBuilder::withInputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = nullptr;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal) {
    return withInputInteger(name, defaultVal, INT_MIN, INT_MAX);
}

NitroNodeBuilder *
NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal, int min, int max) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<IntegerData>(defaultVal);

    // TODO
    auto spinBox = new QSpinBox();
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setValue(defaultVal);
    _displayWrapper->layout()->addWidget(spinBox);

    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputValue(const QString &name, double defaultVal) {
    return withInputValue(name, defaultVal, INT_MIN, INT_MAX);
}

NitroNodeBuilder *
NitroNodeBuilder::withInputValue(const QString &name, double defaultVal, double min, double max) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    // TODO
    auto spinBox = new QDoubleSpinBox();
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setValue(defaultVal);
    _displayWrapper->layout()->addWidget(spinBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name) {
    return withOutputInteger(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name, int defaultVal) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = std::make_shared<IntegerData>(defaultVal);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name) {
    return withOutputValue(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name, double defaultVal) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = std::make_shared<DecimalData>(defaultVal);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDropDown(const QString &name, const QList<QString> &options) {
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withNodeColor(const QColor &color) {
    nodeColor_ = color;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withIcon(const QString &path) {
    iconPath_ = path;
    return this;
}
