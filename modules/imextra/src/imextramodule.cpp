#include "imextramodule.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"
#include "nodes/morphology/morphology.hpp"
#include "nodes/input/kernel.hpp"
#include "nodes/edgedetect/canny.hpp"

namespace nitro::ImExtra {

    ImExtraModule::ImExtraModule() {
    }

    void ImExtraModule::registerNodes(NodeRegistry *registry) {
        registerMorphologyNodes(registry);
        registerEdgeDetectNodes(registry);
    }


    void ImExtraModule::registerMorphologyNodes(NodeRegistry *registry) {
        const QString category = "Morphology";
        registry->registerNode(MorphologyOperator::creator(category));
        registry->registerNode(KernelOperator::creator(category));
    }

    void ImExtraModule::registerEdgeDetectNodes(NodeRegistry *registry) {
        const QString category = "Edge Detection";
        registry->registerNode(CannyEdgeDetectionOperator::creator(category));
    }

} // ImMorph
