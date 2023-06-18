#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ImInfoOperator : public NodeOperator {
    public:
        ImInfoOperator(QLabel *widthLabel, QLabel *heightLabel, QLabel *channelsLabel, QLabel *typeLabel,
                       QLabel *minLabel, QLabel *maxLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        QLabel *widthLabel_;
        QLabel *heightLabel_;
        QLabel *channelsLabel_;
        QLabel *typeLabel_;
        QLabel *minLabel_;
        QLabel *maxLabel_;
    };
} // nitro
