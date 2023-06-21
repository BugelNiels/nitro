#include "imextramodule.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"
#include "src/nodes/filter/morphology.hpp"
#include "nodes/input/structuringelement.hpp"
#include "nodes/input/mask.hpp"
#include "nodes/filter/canny.hpp"
#include "nodes/filter/fouriertransform.hpp"
#include "nodes/filter/dctransform.hpp"
#include "nodes/filter/fouriershift.hpp"
#include "nodes/analysis/distancetransform.hpp"
#include "nodes/segmentation/connectedcomps.hpp"

namespace nitro::ImExtra {

    ImExtraModule::ImExtraModule() {
    }

    void ImExtraModule::registerNodes(NodeRegistry *registry) {
        registerFilterNodes(registry);
        registerAnalysisNodes(registry);
        registerInputNodes(registry);
    }

    void ImExtraModule::registerInputNodes(NodeRegistry *registry) {
        const QString category = "Input";
        registry->registerNode(MaskOperator::creator(category));
        registry->registerNode(StructElemOperator::creator(category));
    }


    void ImExtraModule::registerFilterNodes(NodeRegistry *registry) {
        const QString category = "Filter";
        registry->registerNode(CannyEdgeDetectionOperator::creator(category));
        registry->registerNode(MorphologyOperator::creator(category));
        registry->registerNode(FFTOperator::creator(category));
        registry->registerNode(DCTOperator::creator(category));
        registry->registerNode(FFTShiftOperator::creator(category));
    }

    void ImExtraModule::registerAnalysisNodes(NodeRegistry *registry) {
        const QString category = "Analysis";
        registry->registerNode(DistanceTransformOperator::creator(category));
        registry->registerNode(ConnectedCompsOperator::creator(category));
    }

} // ImMorph
