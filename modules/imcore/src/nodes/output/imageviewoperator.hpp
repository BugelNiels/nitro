#pragma once

#include "nodes/nodeoperator.hpp"
#include "gui/imviewer/imviewer.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ImageViewAlgorithm : public NodeOperator {
    public:

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category, ImageViewer* imageViewer);

        explicit ImageViewAlgorithm(ImageViewer *imViewer);


        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) override;

    private:
        ImageViewer *imViewer_;
    };
}
