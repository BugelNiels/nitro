#include "imcoremodule.hpp"
#include "nodes/operators/image/kmeans.hpp"
#include "nodes/operators/image/flipoperator.hpp"

namespace nitro::ImCore {
    void ImCoreModule::registerImageNodes(NodeRegistry* registry) {
        const QString category = "Image";

        // ------ FLIP Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("FLIP", "flip", category);
            return builder.
                    withOperator(std::make_unique<nitro::FlipOperator>())->
                    withIcon(":/icons/nodes/compare.png")->
                    withNodeColor({118, 185, 0})->
                    withInputImage("Image1")->
                    withInputImage("Image2")->
                    withInputValue("Width (m)", 0.7, 0.1, 5)->
                    withInputInteger("Res (x)", 1920, 1024, 7680)->
                    withInputValue("Dist (m)", 0.7, 0, 5)->
                    withOutputImage("Error Map")->
                    withOutputImage("Errors")->
                    build();
        });

        // ------ Quantization Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
            return builder.
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withCheckBox("Dither")->
                    withInputImage("Image")->
                    withInputInteger("K", 8, 0, 255)->
                    withInputInteger("Order", 5, 0, 32)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ KMeans Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("K-Means", "kMeans", category);
            return builder.
                    withOperator(std::make_unique<nitro::KMeansOperator>())->
                    withIcon(":/icons/nodes/quantisize.png")->
                    withNodeColor({43, 101, 43})->
                    withInputImage("Image")->
                    withInputInteger("K", 8, 1, 255)->
                    withOutputImage("Image")->
                    build();
        });
    }
} // nitro::ImCore
