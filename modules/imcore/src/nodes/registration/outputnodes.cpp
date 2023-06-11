#include "imcoremodule.hpp"
#include "nodes/operators/output/imageviewalgorithm.hpp"

namespace nitro::ImCore {

    void ImCoreModule::registerOutputNodes(NodeRegistry* registry, ImageViewer* imageViewer) {
        const QString category = "Output";

        // ------ Image Viewer Node ------
        registry->registerNode([category, imageViewer]() {
            nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
            return builder.
                    withOperator(std::make_unique<nitro::ImageViewAlgorithm>(imageViewer))->
                    withInputImage("Image")->
                    withIcon(":/icons/nodes/viewer.png")->
                    withNodeColor({60, 29, 38})->
                    build();
        });

    }

} // nitro::ImCore
