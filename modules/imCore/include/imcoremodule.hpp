#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro {
    class RenderView;

    class ImageViewer;

    namespace imCore {

        class ImCoreModule : public NitroModule {
        public:
            ImCoreModule();


            void registerDataTypes(NodeRegistry *registry) override;

            void registerDocks(MainWindow *window) override;

            void registerNodes(NodeRegistry *registry) override;

        private:
            void registerInputNodes(NodeRegistry *registry);

            void registerOutputNodes(NodeRegistry *registry, ImageViewer *imageViewer, RenderView *renderViewer);

            void registerUtilNodes(NodeRegistry *registry);

            void registerImageNodes(NodeRegistry *registry);

            RenderView *renderViewer_;
            ImageViewer *imageViewer_;
        };
    }


} // imCore
