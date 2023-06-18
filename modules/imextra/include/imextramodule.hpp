#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro {
    class ImageViewer;

    namespace ImExtra {

        class ImExtraModule : public NitroModule {
        public:
            ImExtraModule();

            void registerNodes(NodeRegistry *registry) override;

            void registerMorphologyNodes(NodeRegistry *registry);

            void registerEdgeDetectNodes(NodeRegistry *registry);

            void registerFrequencyNodes(NodeRegistry *registry);

            void registerInputNodes(NodeRegistry *registry);
        };
    }


} // imCore
