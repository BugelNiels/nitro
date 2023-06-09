#pragma once

#include <QString>
#include <set>
#include "external/nodeeditor/include/QtNodes/NodeInfo.hpp"
#include "nitronode.hpp"
#include "include/nodes/nodeoperator.hpp"

class QVBoxLayout;

namespace nitro {

    class NitroNodeBuilder {

    public:
        NitroNodeBuilder(QString  name, QString  id, QString  category,
                         const NodeOperator& algo);

        NitroNodeBuilder(QString name, QString id, QString category);

        std::unique_ptr<NitroNode> build();

        // Misc
        NitroNodeBuilder *withNodeColor(const QColor &color);

        NitroNodeBuilder *withIcon(const QString &path);

        // Input
        NitroNodeBuilder *withInputImage(const QString &name);

        NitroNodeBuilder *withInputGreyImage(const QString &name);

        NitroNodeBuilder *withInputColImage(const QString &name);

        NitroNodeBuilder *withInputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder *withInputInteger(const QString &name, int defaultVal, int min, int max);

        NitroNodeBuilder *withInputValue(const QString &name, double defaultVal);

        NitroNodeBuilder *withInputValue(const QString &name, double defaultVal, double min, double max);

        // Output

        NitroNodeBuilder *withOutputGreyImage(const QString &name);

        NitroNodeBuilder *withOutputColImage(const QString &name);

        NitroNodeBuilder *withLoadedOutputImage(const QString &name);

        NitroNodeBuilder *withOutputInteger(const QString &name);

        NitroNodeBuilder *withOutputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder *withOutputValue(const QString &name);

        NitroNodeBuilder *withOutputValue(const QString &name, double defaultVal);

        NitroNodeBuilder *withSourcedOutputInteger(const QString &name);

        NitroNodeBuilder *withSourcedOutputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder *withSourcedOutputValue(const QString &name);

        NitroNodeBuilder *withSourcedOutputValue(const QString &name, double defaultVal);

        // Option Widgets
        NitroNodeBuilder *withDropDown(const QString &name, const QStringList &options);

        NitroNodeBuilder *withCheckBox(const QString &name, bool checked = false);

    private:
        std::unique_ptr<NitroNode> node_;

        const QString name_;
        const QString id_;
        const QString category_;
        const std::shared_ptr<NodeOperator> algo_;
        int portWidgetHeight_;

        // TODO: defaults
        QColor nodeColor_;
        QString iconPath_;

        std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_;
        QVBoxLayout *inLayout_;
        QVBoxLayout *outLayout_;
        QVBoxLayout *optionLayout_;


        void addInPortWidget(QWidget *widget);
        void addInPortWidget(QLabel *label);

        void addOutPortWidget(QWidget *widget);
        void addOutPortWidget(QLabel *label);

        void addOptionWidget(QWidget *widget);


        const int portWidgetWidth_ = 150;

    };

} // nitro
