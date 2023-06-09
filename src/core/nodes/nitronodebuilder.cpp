#include <QVBoxLayout>
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
#include <QCheckBox>
#include <QLabel>

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

using namespace nitro;

NitroNodeBuilder::NitroNodeBuilder(QString name, QString id, QString category,
                                   const NodeOperator &algo)
        : name_(std::move(name)),
          id_(std::move(id)),
          category_(std::move(category)),
          algo_(std::make_unique<NodeOperator>(algo)) {


    // TODO: get these values from the geometry/config
    const int portSpacing = 4;
    portWidgetHeight_ = QFontMetrics(QFont()).height() + 10;

    optionLayout_ = new QVBoxLayout();
    optionLayout_->setSpacing(portSpacing);
    optionLayout_->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    inLayout_ = new QVBoxLayout();
    inLayout_->setSpacing(portSpacing);
    inLayout_->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    outLayout_ = new QVBoxLayout();
    outLayout_->setSpacing(portSpacing);
    outLayout_->setAlignment(Qt::AlignTop | Qt::AlignRight);

    node_ = std::make_unique<NitroNode>();
}


NitroNodeBuilder::NitroNodeBuilder(QString name, QString id, QString category)
        : NitroNodeBuilder(std::move(name), std::move(id), std::move(category), NodeOperator()) {

}

std::unique_ptr<NitroNode> NitroNodeBuilder::build() {
    QtNodes::NodeInfo info(name_, id_, nodeColor_, iconPath_);

    // TODO: perhaps not the best way, but works for now
    QtNodes::NodeColors::registerColor(info);

    NodePorts nodePorts(inputList_, outputList_, inputMap_, outputMap_);

    auto *displayWrapper = new QWidget();
    displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
    auto *vLayout = new QVBoxLayout(displayWrapper);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    if (outLayout_->count() > 0) {
        auto *outWrapper = new QWidget();
        outWrapper->setLayout(outLayout_);
        vLayout->addWidget(outWrapper);
    }

    if (optionLayout_->count() > 0) {
        auto *optionWrapper = new QWidget();
        optionWrapper->setLayout(optionLayout_);
        vLayout->addWidget(optionWrapper);
    }

    if (inLayout_->count() > 0) {
        auto *inWrapper = new QWidget();
        inWrapper->setLayout(inLayout_);
        vLayout->addWidget(inWrapper);
    }

    node_->init(info,
                nodePorts,
                algo_,
                displayWrapper);

    return std::move(node_);
}

void NitroNodeBuilder::addOptionWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    optionLayout_->addWidget(widget, 0, Qt::AlignBottom | Qt::AlignLeft);
}

void NitroNodeBuilder::addInPortWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    inLayout_->addWidget(widget, 0, Qt::AlignBottom | Qt::AlignLeft);
}


void NitroNodeBuilder::addInPortWidget(QLabel *label) {
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    inLayout_->addWidget(label, 0, Qt::AlignBottom | Qt::AlignLeft);
}

void NitroNodeBuilder::addOutPortWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    outLayout_->addWidget(widget, 0, Qt::AlignTop | Qt::AlignRight);
}

void NitroNodeBuilder::addOutPortWidget(QLabel *label) {
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    outLayout_->addWidget(label, 0, Qt::AlignTop | Qt::AlignRight);
}

NitroNodeBuilder *NitroNodeBuilder::withInputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.push_back(item);
    inputMap_[name] = nullptr;
    addInPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputGreyImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.push_back(item);
    inputMap_[name] = nullptr;
    auto portLabel = new QLabel(name);
    addInPortWidget(portLabel);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputColImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ColorImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = nullptr;
    addInPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal) {
    QtNodes::NodeDataType intDataType = IntegerData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, intDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<IntegerData>(defaultVal);


    auto slider = new IntSlider(name, defaultVal);
    node_->connectInputWidget(slider, inputList_.size() - 1);
    addInPortWidget(slider);
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
    addInPortWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withInputValue(const QString &name, double defaultVal) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    auto slider = new DoubleSlider(name, defaultVal);
    node_->connectInputWidget(slider, inputList_.size() - 1);
    addInPortWidget(slider);
    return this;
}

NitroNodeBuilder *
NitroNodeBuilder::withInputValue(const QString &name, double defaultVal, double min, double max) {
    QtNodes::NodeDataType decimalDataType = DecimalData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, decimalDataType);
    inputList_.emplace_back(item);
    inputMap_[name] = std::make_shared<DecimalData>(defaultVal);

    auto slider = new DoubleSlider(name, defaultVal, min, max);
    node_->connectInputWidget(slider, inputList_.size() - 1);
    addInPortWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputGreyImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;
    addOutPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputColImage(const QString &name) {
    QtNodes::NodeDataType imDataType = ColorImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;
    addOutPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withLoadedOutputImage(const QString &name) {
    QtNodes::NodeDataType imDataType = GreyImageData().type();
    auto item = std::pair<QString, QtNodes::NodeDataType>(name, imDataType);
    outputList_.emplace_back(item);
    outputMap_[name] = nullptr;

    auto *loadButton_ = new QPushButton("Load Image");
    loadButton_->setStyleSheet("text-align: left;");
    loadButton_->setIcon(nitro::ImgResourceReader::getPixMap(":/icons/folder_open.png"));
    loadButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loadButton_->adjustSize();
    node_->connectLoadButton(loadButton_, outputList_.size() - 1);
    addOutPortWidget(loadButton_);
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

    addOutPortWidget(new QLabel(name));
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

    addOutPortWidget(new QLabel(name));
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

    auto slider = new IntSlider("", defaultVal);
    node_->connectSourceInteger(slider, outputList_.size() - 1);
    addOutPortWidget(slider);
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

    auto slider = new DoubleSlider("", defaultVal);
    node_->connectSourceValue(slider, outputList_.size() - 1);
    addOutPortWidget(slider);

    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDropDown(const QString &name, const QStringList &options) {
    auto *comboBox = new QComboBox();
    comboBox->addItems(options);
    node_->connectComboBox(name, comboBox);
    addOptionWidget(comboBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withCheckBox(const QString &name, bool checked) {
    auto *checkBox = new QCheckBox(name);
    checkBox->setChecked(checked);
    node_->connectCheckBox(name, checkBox);
    addOptionWidget(checkBox);
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

