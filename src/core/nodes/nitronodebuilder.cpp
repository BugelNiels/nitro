#include <QVBoxLayout>
#include <QSpinBox>
#include <utility>
#include <QPushButton>
#include "nitronodebuilder.hpp"
#include "external/nodeeditor/include/QtNodes/NodeColors.hpp"
#include "util/imgresourcereader.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <QAction>
#include <QComboBox>

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

using namespace nitro;

NitroNodeBuilder::NitroNodeBuilder(const QString name, const QString id, const QString category,
                                   std::shared_ptr<NodeOperator> algo)
        : name_(name),
          id_(id),
          category_(category),
          algo_(std::move(algo)) {

    _displayWrapper = new QWidget();
    _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
    auto *layout = new QVBoxLayout(_displayWrapper);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignRight);

    node_ = std::make_unique<NitroNode>();
}


NitroNodeBuilder::NitroNodeBuilder(const QString name, const QString id, const QString category)
        : NitroNodeBuilder(name, id, category, std::make_shared<NodeOperator>(NodeOperator())) {

}


std::unique_ptr<NitroNode> NitroNodeBuilder::build() {
    QtNodes::NodeInfo info(name_, id_, nodeColor_, iconPath_);

    // TODO: perhaps not the best way, but works for now
    QtNodes::NodeColors::registerColor(info);

    NodePorts nodePorts(inputList_, outputList_, inputMap_, outputMap_);

    node_->init(info,
                nodePorts,
                algo_,
                _displayWrapper);

    return std::move(node_);
}

NitroNodeBuilder *NitroNodeBuilder::withInputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = nullptr;
    _displayWrapper->layout()->addWidget(new QLabel("Image"));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputGreyImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = nullptr;
//    _displayWrapper->layout()->addWidget(new QLabel("Image"));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputColImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ColorImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = nullptr;
//    _displayWrapper->layout()->addWidget(new QLabel("Image"));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<IntegerData>(defaultVal);


    auto slider = new IntSlider(name, defaultVal);
    node_->connectInputWidget(slider, inputList_.size() - 1);
    _displayWrapper->layout()->addWidget(slider);
    return this;
}

NitroNodeBuilder *
NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal, int min, int max) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<IntegerData>(defaultVal);


    auto slider = new IntSlider(name, defaultVal, min, max);
    node_->connectInputWidget(slider, inputList_.size() - 1);
    _displayWrapper->layout()->addWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputValue(const QString &name, double defaultVal) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    auto spinBox = new DoubleSlider(name, defaultVal);
    node_->connectInputWidget(spinBox, inputList_.size() - 1);
    _displayWrapper->layout()->addWidget(spinBox);
    return this;
}

NitroNodeBuilder *
NitroNodeBuilder::withInputValue(const QString &name, double defaultVal, double min, double max) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    auto spinBox = new DoubleSlider(name, defaultVal, min, max);
    node_->connectInputWidget(spinBox, inputList_.size() - 1);
    _displayWrapper->layout()->addWidget(spinBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputGreyImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputColImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ColorImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withLoadedOutputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;


    auto *loadButton_ = new QPushButton("Load Image");
    loadButton_->setIcon(nitro::ImgResourceReader::getPixMap(":/icons/folder_open.png"));
    loadButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loadButton_->adjustSize();
    node_->connectLoadButton(loadButton_, outputList_.size() - 1);
    _displayWrapper->layout()->addWidget(loadButton_);
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

NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputInteger(const QString &name) {
    return withSourcedOutputInteger(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputInteger(const QString &name, int defaultVal) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = std::make_shared<IntegerData>(defaultVal);

    auto slider = new IntSlider(name, defaultVal);
    node_->connectSourceInteger(slider, outputList_.size() - 1);
    _displayWrapper->layout()->addWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputValue(const QString &name) {
    return withSourcedOutputValue(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputValue(const QString &name, double defaultVal) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    auto slider = new DoubleSlider(name, defaultVal);
    node_->connectSourceValue(slider, outputList_.size() - 1);
    _displayWrapper->layout()->addWidget(slider);

    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDropDown(const QString &name, const QStringList &options) {
    auto *comboBox = new QComboBox();
    comboBox->addItems(options);
    node_->connectComboBox(name, comboBox);
    _displayWrapper->layout()->addWidget(comboBox);
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