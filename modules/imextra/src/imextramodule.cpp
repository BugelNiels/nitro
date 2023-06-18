#include "imextramodule.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"
#include "nodes/morphology/morphology.hpp"
#include "nodes/input/structuringelement.hpp"
#include "nodes/edgedetect/canny.hpp"
#include "nodes/fourier/fouriertransform.hpp"
#include "nodes/fourier/fouriershift.hpp"
#include "nodes/input/mask.hpp"

namespace nitro::ImExtra {

    ImExtraModule::ImExtraModule() {
    }

    void ImExtraModule::registerNodes(NodeRegistry *registry) {
        registerMorphologyNodes(registry);
        registerEdgeDetectNodes(registry);
        registerFrequencyNodes(registry);
        registerInputNodes(registry);
    }

    void ImExtraModule::registerInputNodes(NodeRegistry *registry) {
        const QString category = "Input";
        registry->registerNode(MaskOperator::creator(category));
    }


    void ImExtraModule::registerMorphologyNodes(NodeRegistry *registry) {
        const QString category = "Morphology";
        registry->registerNode(MorphologyOperator::creator(category));
        registry->registerNode(StructElemOperator::creator(category));
    }

    void ImExtraModule::registerFrequencyNodes(NodeRegistry *registry) {
        const QString category = "Frequency Domain";
        registry->registerNode(FFTOperator::creator(category));
        registry->registerNode(FFTShift::creator(category));
    }

    void ImExtraModule::registerEdgeDetectNodes(NodeRegistry *registry) {
        const QString category = "Edge Detection";
        registry->registerNode(CannyEdgeDetectionOperator::creator(category));
    }

} // ImMorph
