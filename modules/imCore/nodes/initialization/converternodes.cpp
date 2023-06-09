#include "modules/imCore/imcoremodule.hpp"
#include "modules/imCore/nodes/operators/converter/threshold.hpp"

namespace nitro::imCore {

    void ImCoreModule::buildConverterNodes(NodeRegistry* registry) {
        const QString category = "Converter";

        // ------ Threshold Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Threshold", "threshold", category,
                                            nitro::ThresholdOperator());
            return builder.
                    withIcon(":/icons/nodes/threshold.png")->
                    withNodeColor({43, 101, 43})->
                    withDropDown("Mode", {"<=", ">="})->
                    withInputGreyImage("Image")->
                    withInputInteger("Threshold", 128, 0, 255)->
                    withOutputGreyImage("Image")->
                    build();
        });

        // ------ Separate Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Separate Channels", "separateChannels", category,
                                            nitro::ThresholdOperator());
            return builder.
                    withIcon(":/icons/nodes/separate.png")->
                    withNodeColor({36, 98, 131})->
                    withDropDown("Mode", {"RGB"})->
                    withInputColImage("Image")->
                    withOutputGreyImage("Image")->
                    withOutputGreyImage("Image")->
                    withOutputGreyImage("Image")->
                    build();
        });

        // ------ Combine Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Combine Channels", "combineChannels", category,
                                            nitro::ThresholdOperator());
            return builder.
                    withIcon(":/icons/nodes/combine.png")->
                    withNodeColor({36, 98, 131})->
                    withDropDown("Mode", {"RGB"})->
                    withInputGreyImage("Image")->
                    withInputGreyImage("Image")->
                    withInputGreyImage("Image")->
                    withOutputColImage("Image")->
                    build();
        });
    }
}// nitro::imCore