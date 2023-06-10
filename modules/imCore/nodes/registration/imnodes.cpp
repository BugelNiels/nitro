#include "modules/imCore/imcoremodule.hpp"
#include "modules/imCore/nodes/operators/image/threshold.hpp"

namespace nitro::imCore {
    void ImCoreModule::registerImageNodes(NodeRegistry* registry) {
        const QString category = "Image";

        // ------ Threshold Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Threshold", "threshold", category);
            return builder.
                    withOperator(std::make_unique<nitro::ThresholdOperator>())->
                    withIcon(":/icons/nodes/threshold.png")->
                    withNodeColor({43, 101, 43})->
                    withDropDown("Mode", {"<=", ">="})->
                    withInputGreyImage("Image")->
                    withInputInteger("Threshold", 128, 0, 255)->
                    withOutputGreyImage("Image")->
                    build();
        });

        // ------ FLIP Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("FLIP", "flip", category);
            return builder.
                    withIcon(":/icons/nodes/compare.png")->
                    withNodeColor({118, 185, 0})->
                    withInputImage("Image")->
                    withInputImage("Image")->
                    withOutputColImage("Image")->
                    build();
        });

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

        // ------ Resample Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Resample", "resample", category);
            return builder.
                    withIcon(":/icons/nodes/resample.png")->
                    withNodeColor({201, 94, 6})->
                    withDropDown("Mode", {"Linear", "Cubic"})->
                    withInputGreyImage("Image")->
                    withInputInteger("Levels", 256, 1, 256)->
                    withOutputGreyImage("Image")->
                    build();
        });
    }
} // nitro::imCore