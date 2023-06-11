#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro {
    class RenderView;

    namespace Im3D {

        class Im3DModule : public NitroModule {
        public:
            Im3DModule();


            void registerDataTypes(NodeRegistry *registry) override;

            void registerDocks(MainWindow *window) override;

            void registerNodes(NodeRegistry *registry) override;

        private:

            void registerOutputNodes(NodeRegistry *registry, RenderView *renderViewer);

            void registerImageNodes(NodeRegistry *registry);

            RenderView *renderViewer_;
        };
    }


} // imCore
