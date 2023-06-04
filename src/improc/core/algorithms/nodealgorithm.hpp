#pragma once

#include <map>
#include <QString>
#include "QtNodes/NodeData"

namespace nitro {

    class NodeAlgorithm {
    public:
        virtual void compute(const std::map<QString, std::shared_ptr<QtNodes::NodeData>> &input,
                             std::map<QString, std::shared_ptr<QtNodes::NodeData>> &output,
                             const std::map<QString, QString> &options) const = 0;
    };
}