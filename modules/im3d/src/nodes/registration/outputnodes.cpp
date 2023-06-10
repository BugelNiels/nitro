#include "im3dmodule.hpp"
#include "nodes/operators/output/surfaceviewalgorithm.hpp"

namespace nitro::Im3D {

    void Im3DModule::registerOutputNodes(NodeRegistry* registry, RenderView* renderViewer) {
        const QString category = "Output";

        // ------ Surface Viewer Node ------
        registry->registerNode([category, renderViewer]() {
            nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category);
            return builder.

                    withOperator(std::make_unique<nitro::SurfaceViewAlgorithm>(renderViewer))->
                    withInputImage("image")->
                    withIcon(":/icons/nodes/surface.png")->
                    withNodeColor({99, 28, 28})->
                    build();
        });
    }

} // nitro::ImCore