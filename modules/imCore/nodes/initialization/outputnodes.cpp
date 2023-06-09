#include "modules/imCore/imcoremodule.hpp"
#include "modules/imCore/nodes/operators/output/imageviewalgorithm.hpp"
#include "modules/imCore/nodes/operators/output/surfaceviewalgorithm.hpp"

namespace nitro::imCore {

    void ImCoreModule::buildOutputNodes(NodeRegistry* registry, ImageViewer* imageViewer, RenderView* renderViewer) {
        const QString category = "Output";

        // ------ Image Viewer Node ------
        registry->registerNode([category, imageViewer]() {
            nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category,
                                            nitro::ImageViewAlgorithm(imageViewer));
            return builder.
                    withInputGreyImage("image")->
                    withIcon(":/icons/nodes/viewer.png")->
                    withNodeColor({60, 29, 38})->
                    build();
        });


        // ------ Surface Viewer Node ------
        registry->registerNode([category, renderViewer]() {
            nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category,
                                            nitro::SurfaceViewAlgorithm(renderViewer));
            return builder.
                    withInputGreyImage("image")->
                    withIcon(":/icons/nodes/surface.png")->
                    withNodeColor({99, 28, 28})->
                    build();
        });
    }

} // nitro::imCore