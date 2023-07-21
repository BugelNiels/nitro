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

            void registerFilterNodes(NodeRegistry *registry);

            void registerAnalysisNodes(NodeRegistry *registry);

            void registerInputNodes(NodeRegistry *registry);
        };
    }


} // imCore
