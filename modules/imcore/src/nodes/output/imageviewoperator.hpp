#pragma once

#include "nodes/nodeoperator.hpp"
#include "gui/imviewer/imviewer.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ImageViewOperator : public NodeOperator {
    public:

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category, ImageViewer* imageViewer);

        explicit ImageViewOperator(ImageViewer *imViewer);


        void execute(NodePorts &nodePorts) override;

    private:
        ImageViewer *imViewer_;
    };
}
