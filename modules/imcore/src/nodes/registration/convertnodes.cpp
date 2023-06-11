
#include <opencv2/imgproc.hpp>
#include "imcoremodule.hpp"

#include "nodes/noderegistry.hpp"
#include "nodes/operators/convert/convertoperator.hpp"
#include "nodes/operators/convert/separateoperator.hpp"
#include "nodes/operators/convert/combineoperator.hpp"
#include "nodes/operators/convert/grayscaleconvert.hpp"
#include "nodes/operators/convert/mixoperator.hpp"
#include "nodes/operators/convert/mathoperator.hpp"

namespace nitro::ImCore {

    void ImCoreModule::registerConvertNodes(NodeRegistry *registry) {
        const QString category = "Convert";

        // ------ Mix Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Mix", "mix", category);
            return builder.
                    withOperator(std::make_unique<nitro::MixOperator>())->
                    withIcon(":/icons/nodes/mix.png")->
                    withNodeColor({110, 110, 29})->
                    withDropDown("Mode", {"Mix", "Add", "Subtract", "Multiply"})->
                    withInputValue("Fac", 0.5, 0, 1)->
                    withInputImage("Image1")->
                    withInputImage("Image2")->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Math Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Math", "math", category);
            return builder.
                    withOperator(std::make_unique<nitro::MathOperator>())->
                    withIcon(":/icons/nodes/math.png")->
                    withNodeColor({110, 110, 29})->
                    withDropDown("Mode", {"Add", "Subtract", "Multiply", "Divide"})->
                    withInputValue("Fac", 0.5, 0, 1)->
                    withInputImage("Image")->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Separate Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Separate Channels", "separateChannels", category);
            return builder.
                    withOperator(std::make_unique<nitro::SeparateOperator>())->
                    withIcon(":/icons/nodes/separate.png")->
                    withNodeColor({36, 98, 131})->
                    withInputImage("Image")->
                    withOutputImage("Channel1")->
                    withOutputImage("Channel2")->
                    withOutputImage("Channel3")->
                    build();
        });

        // ------ Combine Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Combine Channels", "combineChannels", category);
            return builder.
                    withOperator(std::make_unique<nitro::CombineOperator>())->
                    withIcon(":/icons/nodes/combine.png")->
                    withNodeColor({36, 98, 131})->
                    withInputImage("Channel1")->
                    withInputImage("Channel2")->
                    withInputImage("Channel3")->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Convert Node ------
        registry->registerNode([category]() {
            QStringList colorNames;
            std::vector<cv::ColorConversionCodes> codes;
            colorNames.append("RGB -> XYZ");
            codes.push_back(cv::COLOR_RGB2XYZ);

            colorNames.append("XYZ -> RGB");
            codes.push_back(cv::COLOR_XYZ2RGB);

            colorNames.append("RGB -> YCrCb");
            codes.push_back(cv::COLOR_RGB2YCrCb);

            colorNames.append("YCrCb -> RGB");
            codes.push_back(cv::COLOR_YCrCb2RGB);

            colorNames.append("RGB -> HSV");
            codes.push_back(cv::COLOR_RGB2HSV);

            colorNames.append("HSV -> RGB");
            codes.push_back(cv::COLOR_HSV2RGB);

            colorNames.append("RGB -> LAB");
            codes.push_back(cv::COLOR_RGB2Lab);

            colorNames.append("LAB -> RGB");
            codes.push_back(cv::COLOR_Lab2RGB);

            nitro::NitroNodeBuilder builder("Convert", "convert", category);
            return builder.
                    withOperator(std::make_unique<nitro::ConvertOperator>(codes))->
                    withIcon(":/icons/nodes/convert.png")->
                    withNodeColor({36, 98, 131})->
                    withDropDown("Mode", colorNames)->
                    withInputImage("Image")->
                    withOutputImage("Image")->
                    build();
        });

        // ------ Grayscale Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Grayscale Convert", "grayscaleConvert", category);
            return builder.
                    withOperator(std::make_unique<nitro::GrayscaleConvert>())->
                    withIcon(":/icons/nodes/greyscale.png")->
                    withNodeColor({36, 98, 131})->
                    withInputImage("Image")->
                    withOutputImage("Image")->
                    build();
        });
    }
}// nitro::ImCore
