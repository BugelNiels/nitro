#pragma once

#include "include/nodes/nodeoperator.hpp"
#include "modules/imCore/gui/imviewer/imviewer.hpp"

namespace nitro {

    class ImageViewAlgorithm : public NodeOperator {
    public:
        explicit ImageViewAlgorithm(ImageViewer *imViewer);


        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        ImageViewer *imViewer_;
    };
}