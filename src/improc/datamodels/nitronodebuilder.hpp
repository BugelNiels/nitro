#pragma once

#include <QString>
#include "nitronode.hpp"
#include "src/improc/core/algorithms/nodealgorithm.hpp"
#include "nodes/nodeinfo.hpp"

namespace nitro {

    class NitroNodeBuilder {


    public:
        NitroNodeBuilder(const QString name, const QString id, const QString category, std::shared_ptr<NodeAlgorithm> algo);

        std::unique_ptr<NitroNode> build();

        // Misc
        NitroNodeBuilder* withNodeColor(const QColor &color);
        NitroNodeBuilder* withIcon(const QString &path);

        // Input
        NitroNodeBuilder* withInputImage(const QString &name);

        NitroNodeBuilder* withInputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder* withInputInteger(const QString &name, int defaultVal, int min, int max);

        NitroNodeBuilder* withInputValue(const QString &name, double defaultVal);

        NitroNodeBuilder* withInputValue(const QString &name, double defaultVal, double min, double max);

        // Output

        NitroNodeBuilder* withOutputImage(const QString &name);

        NitroNodeBuilder* withOutputInteger(const QString &name);
        NitroNodeBuilder* withOutputInteger(const QString &name, int defaultVal);

        NitroNodeBuilder* withOutputValue(const QString &name);

        NitroNodeBuilder* withOutputValue(const QString &name, double defaultVal);

        // Widget
        NitroNodeBuilder* withDropDown(const QString &name, const QList<QString> &options);
    private:

        const QString name_;
        const QString id_;
        const QString category_;
        const std::shared_ptr<NodeAlgorithm> algo_;

        // TODO: defaults
        QColor nodeColor_;
        QString iconPath_;

        std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_;

        QWidget *_displayWrapper;

    };

} // nitro
