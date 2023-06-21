#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro {
    class ImageViewer;

    namespace ImCore {

        class ImCoreModule : public NitroModule {
        public:
            ImCoreModule();

            void registerDataTypes(NodeRegistry *registry) override;

            void registerDocks(MainWindow *window) override;

            void registerNodes(NodeRegistry *registry) override;

        private:
            void registerInputNodes(NodeRegistry *registry);

            void registerOutputNodes(NodeRegistry *registry, ImageViewer *imageViewer);

            void registerConvertNodes(NodeRegistry *registry);

            void registerFilterNodes(NodeRegistry *registry);

            void registerCompressionNodes(NodeRegistry *registry);

            ImageViewer *imageViewer_;

            void registerColorNodes(NodeRegistry *registry);

            void registerQualityMetricNodes(NodeRegistry *registry);

            void registerTransformNodes(NodeRegistry *registry);

            void registerRestorationNodes(NodeRegistry *registry);
        };
    }


} // imCore
