#pragma once

#include "nitromodule.hpp"
#include "modules/imCore/gui/imviewer/imviewer.hpp"
#include "modules/imCore/gui/im3dviewer/renderview.hpp"

namespace nitro::imCore {

    class ImCoreModule : public NitroModule {
    public:
        ImCoreModule();


        void registerDataTypes(NodeRegistry* registry) override;
        void registerDocks(nitro::MainWindow *window) override;
        void registerNodes(NodeRegistry* registry) override;

    private:
        void registerInputNodes(NodeRegistry* registry);
        void registerOutputNodes(NodeRegistry* registry, ImageViewer* imageViewer, RenderView* renderViewer);
        void registerUtilNodes(NodeRegistry* registry);
        void registerImageNodes(NodeRegistry* registry);

        RenderView *renderViewer_;
        ImageViewer *imageViewer_;
    };

} // imCore
