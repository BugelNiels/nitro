#include "modules/imCore/imcoremodule.hpp"

namespace nitro::imCore {

    void ImCoreModule::buildResampleNodes(NodeRegistry* registry) {
        const QString category = "Resampling";

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