#include "imcoremodule.hpp"

namespace nitro::ImCore {

    void ImCoreModule::registerInputNodes(NodeRegistry* registry) {
        const QString category = "Input";

        // ------ Image Source Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Image Source", "ImageSource", category);
            return builder.
                    withLoadedOutputImage("image")->
                    withIcon(":/icons/nodes/image_source.png")->
                    withNodeColor({121, 70, 29})->
                    build();
        });

        // ------ Decimal Source Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Value", "ValueSource", category);
            return builder.
                    withSourcedOutputValue("Value")->
                    withIcon(":/icons/nodes/number.png")->
                    withNodeColor({131, 49, 74})->
                    build();
        });

        // ------ Integer Source Node ------
        registry->registerNode([category]() {
            nitro::NitroNodeBuilder builder("Integer", "IntegerSource", category);
            return builder.
                    withSourcedOutputInteger("Integer")->
                    withIcon(":/icons/nodes/number.png")->
                    withNodeColor({131, 49, 74})->
                    build();
        });
    }

} // nitro::ImCore