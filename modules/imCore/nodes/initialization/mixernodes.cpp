#include "modules/imCore/imcoremodule.hpp"

namespace nitro::imCore {

    void ImCoreModule::buildComparisonNodes(NodeRegistry* registry) {
        const QString category = "Comparison";

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

    }
} // nitro::imCore