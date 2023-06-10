#include "modules/imCore/imcoremodule.hpp"
#include "modules/imCore/nodes/operators/output/imageviewalgorithm.hpp"
#include "modules/imCore/nodes/operators/output/surfaceviewalgorithm.hpp"

namespace nitro::imCore {

    void ImCoreModule::registerOutputNodes(NodeRegistry* registry, ImageViewer* imageViewer, RenderView* renderViewer) {
        const QString category = "Output";

        qDebug() << imageViewer;
        // ------ Image Viewer Node ------
        registry->registerNode([category, imageViewer]() {
            nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
            return builder.
                    withOperator(std::make_unique<nitro::ImageViewAlgorithm>(imageViewer))->
                    withInputGreyImage("image")->
                    withIcon(":/icons/nodes/viewer.png")->
                    withNodeColor({60, 29, 38})->
                    build();
        });


        // ------ Surface Viewer Node ------
        registry->registerNode([category, renderViewer]() {
            nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
            return builder.

                    withOperator(std::make_unique<nitro::SurfaceViewAlgorithm>(renderViewer))->
                    withInputGreyImage("image")->
                    withIcon(":/icons/nodes/surface.png")->
                    withNodeColor({99, 28, 28})->
                    build();
        });
    }

} // nitro::imCore