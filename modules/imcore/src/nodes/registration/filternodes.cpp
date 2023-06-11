#include "imcoremodule.hpp"

#include <opencv2/imgproc.hpp>

#include "nodes/noderegistry.hpp"
#include "nodes/operators/image/threshold.hpp"
#include "nodes/operators/convert/mixoperator.hpp"
#include "nodes/operators/filters/boxfilteroperator.hpp"
#include "nodes/operators/filters/denoiseoperator.hpp"
#include "nodes/operators/filters/gaussianbluroperator.hpp"
#include "nodes/operators/filters/bilateralfilteroperator.hpp"

namespace nitro::ImCore {

    void ImCoreModule::registerFilterNodes(NodeRegistry *registry) {
        const QString category = "Filter";

        // ------ Threshold Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Threshold", "threshold", category);
            return builder.
                    withOperator(std::make_unique<nitro::ThresholdOperator>())->
                    withIcon(":/icons/nodes/threshold.png")->
                    withNodeColor({43, 101, 43})->
                    withDropDown("Mode", {"<=", ">="})->
                    withInputImage("Image")->
                    withInputInteger("Threshold", 128, 0, 255)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Blur Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Box Filter", "boxFilter", category);
            return builder.
                    withOperator(std::make_unique<nitro::BoxFilterOperator>())->
                    withIcon(":/icons/nodes/blur.png")->
                    withNodeColor({43, 101, 43})->
                    withDropDown("Mode", {"Average", "Median"})->
                    withInputImage("Image")->
                    withInputInteger("Size", 5, 1, 256)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Gaussian Blur Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Gaussian Blur", "gaussianFilter", category);
            return builder.
                    withOperator(std::make_unique<nitro::GaussianBlurOperator>())->
                    withIcon(":/icons/nodes/blur.png")->
                    withNodeColor({43, 101, 43})->
                    withInputImage("Image")->
                    withInputInteger("Size", 5, 1, 256)->
                    withInputValue("Sigma", 1, 0, 16)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Bilateral Blur Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
            return builder.
                    withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                    withIcon(":/icons/nodes/blur.png")->
                    withNodeColor({43, 101, 43})->
                    withInputImage("Image")->
                    withInputInteger("d", 9, 1, 64)->
                    withInputValue("Sigma (c)", 75, 1, 255)->
                    withInputValue("Sigma (s)", 75, 1, 255)->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Denoising Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Denoise", "denoise", category);
            return builder.
                    withOperator(std::make_unique<nitro::DenoiseOperator>())->
                    withIcon(":/icons/nodes/blur.png")->
                    withNodeColor({43, 101, 43})->
                    withInputImage("Image")->
                    withOutputImage("Image")->
                    build();
        });


    }
}// nitro::ImCore
