#include "im3dmodule.hpp"

namespace nitro::Im3D {
    void Im3DModule::registerImageNodes(NodeRegistry* registry) {
        const QString category = "Image";

        // ------ Layer Removal Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Layer Removal", "layerRemoval", category);
            return builder.
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withCheckBox("Dither")->
                    withInputImage("Image")->
                    withInputInteger("K", 8, 0, 255)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Resample Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Resample", "resample", category);
            return builder.
                    withIcon(":/icons/nodes/resample.png")->
                    withNodeColor({201, 94, 6})->
                    withDropDown("Mode", {"Linear", "Cubic"})->
                    withInputImage("Image")->
                    withInputInteger("Levels", 256, 1, 256)->
                    withOutputImage("Image")->
                    build();
        });
    }
} // nitro::ImCore