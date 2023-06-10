#include "modules/imCore/imcoremodule.hpp"
#include "modules/imCore/nodes/operators/image/threshold.hpp"

namespace nitro::imCore {

    void ImCoreModule::registerUtilNodes(NodeRegistry* registry) {
        const QString category = "Util";

        // ------ Mix Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Mix", "mix", category);
            return builder.
                    withIcon(":/icons/nodes/mix.png")->
                    withNodeColor({110, 110, 29})->
                    withDropDown("Mode", {"Mix", "Add"})->
                    withInputValue("Fac", 0.5, 0, 1)->
                    withInputImage("Image1")->
                    withInputImage("Image2")->
                    withOutputColImage("Image")->
                    build();
        });

        // ------ Separate Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Separate Channels", "separateChannels", category);
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
            nitro::NitroNodeBuilder builder("Combine Channels", "combineChannels", category);
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