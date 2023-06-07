#pragma once

#include "src/core/nodes/operators/nodeoperator.hpp"
#include "ui/imgviewer/imgviewer.hpp"

namespace nitro {

    class ImageViewAlgorithm : public NodeOperator {
    public:
        explicit ImageViewAlgorithm(ImageViewer *imViewer);


        void execute(NodePorts &nodePorts, const std::map<QString, QString> &options) const override;

    private:
        ImageViewer *imViewer_;
    };
}