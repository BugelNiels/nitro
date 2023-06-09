#pragma once

#include "nitromodule.hpp"
#include "modules/imCore/gui/imgviewer/imgviewer.hpp"
#include "modules/imCore/gui/surfacevis/renderview.hpp"

namespace nitro::imCore {

    class ImCoreModule : public NitroModule {
    public:
        ImCoreModule();


        void registerDataTypes(NodeRegistry* registry) override;
        void registerDocks(nitro::MainWindow *window) override;
        void registerNodes(NodeRegistry* registry) override;

    private:
        void buildInputNodes(NodeRegistry* registry);
        void buildOutputNodes(NodeRegistry* registry, ImageViewer* imageViewer, RenderView* renderViewer);
        void buildConverterNodes(NodeRegistry* registry);
        void buildComparisonNodes(NodeRegistry* registry);
        void buildFilterNodes(NodeRegistry* registry);
        void buildColorNodes(NodeRegistry* registry);
        void buildQuantizationNodes(NodeRegistry* registry);
        void buildResampleNodes(NodeRegistry* registry);

        RenderView *renderViewer_;
        ImageViewer *imageViewer_;
    };

} // imCore
