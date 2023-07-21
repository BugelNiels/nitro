#include <nodes/nitronodebuilder.hpp>

#include <QVBoxLayout>
#include <utility>
#include <QPushButton>
#include <QtNodes/NodeColors.hpp>
#include <QAction>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>


#include "src/util/imgresourcereader.hpp"
#include "colimagedata.hpp"
#include <nodes/datatypes/integerdata.hpp>
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/nitronode.hpp>
#include <doubleslider.hpp>
#include <intslider.hpp>
#include <QAbstractItemView>

using namespace nitro;

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

static QWidget *createWrapper() {
    auto *wrapper = new QWidget();
    wrapper->setContentsMargins(0, 0, 0, 0);
    auto *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    wrapper->setLayout(layout);
    return wrapper;
}

NitroNodeBuilder::NitroNodeBuilder(QString name, QString id, QString category)
        : name_(std::move(name)),
          id_(std::move(id)),
          category_(std::move(category)) {
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

std::unique_ptr<NitroNode> NitroNodeBuilder::build() {
    QtNodes::NodeInfo info(name_, id_, category_, nodeColor_, iconPath_);
    QtNodes::NodeColors::registerColor(info);

    NodePorts nodePorts(inputList_, outputList_, options_);

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
                std::move(algo_),
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

NitroNodeBuilder *
NitroNodeBuilder::withInputInteger(const QString &name, int defaultVal, int min, int max,
                                   BoundMode boundMode,
                                   std::initializer_list<QString> conversionTypes) {
    initInputValue(name, new IntSlider(name, defaultVal, min, max, boundMode), conversionTypes);
    return this;
}

void NitroNodeBuilder::initInputValue(const QString &name, ValueSliders::IntSlider *slider,
                                      std::initializer_list<QString> cTypes) {
    auto data = std::make_shared<IntegerData>(slider->getVal());
    for (auto &cType: cTypes) {
        data->allowConversionFrom(cType);
    }
    inputList_.emplace_back(name, data);

    auto valueLabel = new QLabel(name);
    node_->connectInputWidget(slider, valueLabel, inputList_.size() - 1);
    QWidget *wrapper = createWrapper();
    wrapper->layout()->addWidget(slider);
    wrapper->layout()->addWidget(valueLabel);
    valueLabel->setHidden(true);
    addInPortWidget(wrapper);
}

void NitroNodeBuilder::initInputVal(const QString &name, ValueSliders::DoubleSlider *slider,
                                    std::initializer_list<QString> cTypes) {
    auto data = std::make_shared<DecimalData>(slider->getVal());
    for (auto &cType: cTypes) {
        data->allowConversionFrom(cType);
    }
    inputList_.emplace_back(name, data);


    auto valueLabel = new QLabel(name);
    node_->connectInputWidget(slider, valueLabel, inputList_.size() - 1);
    QWidget *wrapper = createWrapper();
    wrapper->layout()->addWidget(slider);
    wrapper->layout()->addWidget(valueLabel);
    valueLabel->setHidden(true);
    addInPortWidget(wrapper);
}

NitroNodeBuilder *
NitroNodeBuilder::withInputValue(const QString &name, double defaultVal, double min, double max,
                                 BoundMode boundMode,
                                 std::initializer_list<QString> conversionTypes) {
    initInputVal(name, new DoubleSlider(name, defaultVal, min, max, boundMode), conversionTypes);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withLoadButton(const QString &name, const QString &filters) {
    // TODO: remove dependency on colimagedata
    outputList_.emplace_back(name, std::make_shared<ColImageData>());
    // TODO: align icon
    auto *loadButton = new QPushButton();
    loadButton->setIcon(nitro::ImResourceReader::getPixMap(":/icons/folder_open.png"));
    loadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loadButton->adjustSize();
    node_->connectLoadButton(name, loadButton, outputList_.size() - 1, filters);
    addOutPortWidget(new QLabel("Image"));
    addOptionWidget(loadButton);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name) {
    return withOutputInteger(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name, int defaultVal) {
    outputList_.emplace_back(name, std::make_shared<IntegerData>(defaultVal));

    addOutPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name) {
    return withOutputValue(name, 0);
}


NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name, double defaultVal) {
    outputList_.emplace_back(name, std::make_shared<DecimalData>(defaultVal));
    addOutPortWidget(new QLabel(name));
    return this;
}


NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputInteger(const QString &name, int defaultVal, int min, int max,
                                                             BoundMode boundMode) {
    outputList_.emplace_back(name, std::make_shared<IntegerData>(defaultVal));

    auto slider = new IntSlider("", defaultVal, min, max, boundMode);
    node_->connectSourceInteger(slider, outputList_.size() - 1);
    addOutPortWidget(slider);
    return this;
}

NitroNodeBuilder *
NitroNodeBuilder::withSourcedOutputValue(const QString &name, double defaultVal, double min, double max,
                                         BoundMode boundMode) {
    outputList_.emplace_back(name, std::make_shared<DecimalData>(defaultVal));

    auto slider = new DoubleSlider("", defaultVal, min, max, boundMode);
    node_->connectSourceValue(slider, outputList_.size() - 1);
    addOutPortWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDropDown(const QString &name, const QStringList &options) {
    auto *comboBox = new QComboBox();
    comboBox->addItems(options);

    // TODO: ensure scrollbar for too many items
    options_[name] = comboBox->currentIndex();
    node_->connectComboBox(name, comboBox);
    addOptionWidget(comboBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withCheckBox(const QString &name, bool checked) {
    auto *checkBox = new QCheckBox(name);
    checkBox->setChecked(checked);
    options_[name] = checkBox->isChecked();
    node_->connectCheckBox(name, checkBox);
    addOptionWidget(checkBox);
    return this;
}


NitroNodeBuilder *NitroNodeBuilder::withDisplayWidget(const QString &name, QLabel *label) {
    node_->connectLabel(name, label);
    addOptionWidget(label);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDisplayWidget(const QString &name, const QString &text) {
    auto *label = new QLabel(text);
    node_->connectLabel(name, label);
    addOptionWidget(label);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withNodeColor(const QColor &color) {
    nodeColor_ = color;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withIcon(const QString &path) {
    iconPath_ = QString(":/icons/nodes/%1").arg(path);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOperator(std::unique_ptr<NodeOperator> algo) {
    algo_ = std::move(algo);
    return this;
}

