#pragma once

#include <QString>
#include <set>
#include <QColor>
#include <QLabel>
#include "nodeoperator.hpp"
#include "nitronode.hpp"
#include "include/valueslider.hpp"

class QVBoxLayout;

using BoundMode = ValueSliders::BoundMode;

namespace nitro {

class NitroNodeBuilder {

public:
    NitroNodeBuilder(QString name, QString id, QString category);

    std::unique_ptr<NitroNode> build();

    NitroNodeBuilder *withOperator(std::unique_ptr<NodeOperator> algo);

    // Misc
    NitroNodeBuilder *withNodeColor(const QColor &color);

    NitroNodeBuilder *withIcon(const QString &path);

    template<class T>
    NitroNodeBuilder *withInputPort(const QString &name) {
        QtNodes::NodeDataType imDataType = T().type();
        inputList_.emplace_back(name, imDataType);
        addInPortWidget(new QLabel(name));
        return this;
    }

    template<class T>
    NitroNodeBuilder *withInputPort(const QString &name, std::initializer_list<QString> allowedConversions) {
        QtNodes::NodeDataType imDataType = T().type();
        for (auto &type: allowedConversions) {
            imDataType.allowedFromConversions.insert(type);
        }
        inputList_.emplace_back(name, imDataType);
        addInPortWidget(new QLabel(name));
        return this;
    }

    // TODO: refactor this with fewer parameters

    NitroNodeBuilder *withInputInteger(const QString &name, int defaultVal, int min, int max,
                                       BoundMode boundMode = BoundMode::UPPER_LOWER,
                                       std::initializer_list<QString> allowedConversions = {});

    NitroNodeBuilder *withInputValue(const QString &name, double defaultVal, double min, double max,
                                     BoundMode boundMode = BoundMode::UPPER_LOWER,
                                     std::initializer_list<QString> allowedConversions = {});

    // Output
    template<class T>
    NitroNodeBuilder *withOutputPort(const QString &name) {
        QtNodes::NodeDataType imDataType = T().type();
        outputList_.emplace_back(name, imDataType);
        addOutPortWidget(new QLabel(name));
        return this;
    }

    NitroNodeBuilder *withLoadButton(const QString &name, const QString &filters);

    NitroNodeBuilder *withOutputInteger(const QString &name);

    NitroNodeBuilder *withOutputInteger(const QString &name, int defaultVal);

    NitroNodeBuilder *withOutputValue(const QString &name);

    NitroNodeBuilder *withOutputValue(const QString &name, double defaultVal);

    NitroNodeBuilder *withSourcedOutputInteger(const QString &name, int defaultVal, int min, int max,
                                               BoundMode boundMode = BoundMode::UPPER_LOWER);

    NitroNodeBuilder *withSourcedOutputValue(const QString &name, double defaultVal, double min, double max,
                                             BoundMode boundMode = BoundMode::UPPER_LOWER);

    // Option Widgets
    NitroNodeBuilder *withDropDown(const QString &name, const QStringList &options);

    NitroNodeBuilder *withCheckBox(const QString &name, bool checked = false);

    NitroNodeBuilder *withDisplayWidget(const QString &name, QLabel *label);

    NitroNodeBuilder *withDisplayWidget(const QString &name, const QString &text);

private:
    const int portWidgetWidth_ = 150;

    const QString name_;
    const QString id_;
    const QString category_;
    std::unique_ptr<NodeOperator> algo_;
    std::unique_ptr<NitroNode> node_;
    int portWidgetHeight_;

    QColor nodeColor_ = NITRO_CONVERTER_COLOR;
    QString iconPath_;

    std::vector<PortData> inputList_;
    std::vector<PortData> outputList_;
    std::unordered_map<QString, int> options_;
    QVBoxLayout *inLayout_;
    QVBoxLayout *outLayout_;
    QVBoxLayout *optionLayout_;


    void addInPortWidget(QWidget *widget);

    void addInPortWidget(QLabel *label);

    void addOutPortWidget(QWidget *widget);

    void addOutPortWidget(QLabel *label);

    void addOptionWidget(QWidget *widget);


    void initInputValue(const QString &name, ValueSliders::IntSlider *slider,
                        std::initializer_list<QString> list);

    void initInputVal(const QString &name, ValueSliders::DoubleSlider *slider,
                      std::initializer_list<QString> list);

};

} // namespace nitro
