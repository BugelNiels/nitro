#pragma once

#include <QString>
#include <set>
#include <QColor>
#include <QLabel>
#include "nodeoperator.hpp"
#include "nitronode.hpp"
#include "datainfo.hpp"
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
            nitro::DataInfo info = T::dataInfo();
            QtNodes::NodeDataType imDataType = {info.getDataId(), info.getDataName(), info.getDataColor()};
            // TODO: do this automatically
            imDataType.allowedConversion.insert("Decimal");
            inputList_.emplace_back(name, imDataType);
            addInPortWidget(new QLabel(name));
            return this;
        }

        NitroNodeBuilder *withInputInteger(const QString &name, int defaultVal, int bound,
                                           BoundMode boundMode = BoundMode::LOWER_ONLY);

        NitroNodeBuilder *withInputInteger(const QString &name, int defaultVal, int min, int max,
                                           BoundMode boundMode = BoundMode::UPPER_LOWER);

        NitroNodeBuilder *withInputValue(const QString &name, double defaultVal, double bound,
                                         BoundMode boundMode = BoundMode::LOWER_ONLY);

        NitroNodeBuilder *withInputValue(const QString &name, double defaultVal, double min, double max,
                                         BoundMode boundMode = BoundMode::UPPER_LOWER);

        // Output
        template<class T>
        NitroNodeBuilder *withOutputPort(const QString &name) {
            nitro::DataInfo info = T::dataInfo();
            QtNodes::NodeDataType imDataType = {info.getDataId(), info.getDataName(), info.getDataColor()};
            outputList_.emplace_back(name, imDataType);
            addOutPortWidget(new QLabel(name));
            return this;
        }

        NitroNodeBuilder *withLoadButton(const QString &name, const QString &filters);

        NitroNodeBuilder *withOutputInteger(const QString &name);

        NitroNodeBuilder *withOutputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder *withOutputValue(const QString &name);

        NitroNodeBuilder *withOutputValue(const QString &name, double defaultVal);

        NitroNodeBuilder *withSourcedOutputInteger(const QString &name, int defaultVal, int bound,
                                                   BoundMode boundMode = BoundMode::LOWER_ONLY);

        NitroNodeBuilder *withSourcedOutputInteger(const QString &name, int defaultVal, int min, int max,
                                                   BoundMode boundMode = BoundMode::UPPER_LOWER);


        NitroNodeBuilder *withSourcedOutputValue(const QString &name, double defaultVal, double bound,
                                                   BoundMode boundMode = BoundMode::LOWER_ONLY);

        NitroNodeBuilder *withSourcedOutputValue(const QString &name, double defaultVal, double min, double max,
                                                   BoundMode boundMode = BoundMode::UPPER_LOWER);

        // Option Widgets
        NitroNodeBuilder *withDropDown(const QString &name, const QStringList &options);

        NitroNodeBuilder *withCheckBox(const QString &name, bool checked = false);

        NitroNodeBuilder *withDisplayWidget(const QString &name, QLabel *label);

    private:
        const int portWidgetWidth_ = 150;

        const QString name_;
        const QString id_;
        const QString category_;
        std::unique_ptr<NodeOperator> algo_;
        std::unique_ptr<NitroNode> node_;
        int portWidgetHeight_;

        QColor nodeColor_ = {36, 98, 131};
        QString iconPath_;

        std::vector<PortData> inputList_;
        std::vector<PortData> outputList_;
        QVBoxLayout *inLayout_;
        QVBoxLayout *outLayout_;
        QVBoxLayout *optionLayout_;


        void addInPortWidget(QWidget *widget);

        void addInPortWidget(QLabel *label);

        void addOutPortWidget(QWidget *widget);

        void addOutPortWidget(QLabel *label);

        void addOptionWidget(QWidget *widget);


    };

} // nitro
