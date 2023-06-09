#include "modules/imCore/imcoremodule.hpp"

namespace nitro::imCore {

    void ImCoreModule::buildQuantizationNodes(NodeRegistry* registry) {
        const QString category = "Quantization";

        // ------ Quantization Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
            return builder.
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withCheckBox("Dither")->
                    withInputGreyImage("Image")->
                    withInputInteger("K", 8, 0, 255)->
                    withInputInteger("Order", 5, 0, 32)->
                    withOutputGreyImage("Image")->
                    build();
        });

        // ------ KMeans Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("K-Means", "kMeans", category);
            return builder.
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withCheckBox("Dither")->
                    withInputGreyImage("Image")->
                    withInputInteger("K", 8, 0, 255)->
                    withInputInteger("Iterations", 16, 1, 32)->
                    withOutputGreyImage("Image")->
                    build();
        });

        // ------ Layer Removal Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Layer Removal", "layerRemoval", category);
            return builder.
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withCheckBox("Dither")->
                    withInputGreyImage("Image")->
                    withInputInteger("K", 8, 0, 255)->
                    withOutputGreyImage("Image")->
                    build();
        });
    }

} // nitro::imCore