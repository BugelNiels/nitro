#include "improc.hpp"

#include "nodes/analysis/distancetransform.hpp"
#include "nodes/blur/bilateralfilter.hpp"
#include "nodes/blur/boxfilter.hpp"
#include "nodes/blur/gaussianblur.hpp"
#include "nodes/filter/canny.hpp"
#include "nodes/filter/dctransform.hpp"
#include "nodes/filter/fouriershift.hpp"
#include "nodes/filter/fouriertransform.hpp"
#include "nodes/input/mask.hpp"
#include "nodes/input/structuringelement.hpp"
#include "nodes/quality/flip.hpp"
#include "nodes/quality/mse.hpp"
#include "nodes/quality/psnr.hpp"
#include "nodes/quantization/kmeans.hpp"
#include "nodes/quantization/quantize.hpp"
#include "nodes/restoration/denoise.hpp"
#include "nodes/segmentation/connectedcomps.hpp"
#include "src/nodes/filter/morphology.hpp"
#include <gui/mainwindow.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::ImProc {

ImProc::ImProc() = default;

void ImProc::registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) {
    Q_UNUSED(window);
    registerFilterNodes(registry);
    registerAnalysisNodes(registry);
    registerInputNodes(registry);
    registerQualityMetricNodes(registry);
    registerRestorationNodes(registry);
    registerQuantizationNodes(registry);
}

void ImProc::registerInputNodes(std::shared_ptr<NodeRegistry> &registry) const {
    const QString category = "Input";
    registry->registerNode(MaskOperator::creator(category));
    registry->registerNode(StructElemOperator::creator(category));
}

void ImProc::registerFilterNodes(std::shared_ptr<NodeRegistry> &registry) const {
    const QString category = "Filter";
    registry->registerNode(BoxFilterOperator::creator(category));
    registry->registerNode(GaussianBlurOperator::creator(category));
    registry->registerNode(BilateralFilterOperator::creator(category));
    registry->registerNode(CannyEdgeDetectionOperator::creator(category));
    registry->registerNode(MorphologyOperator::creator(category));
    registry->registerNode(FFTOperator::creator(category));
    registry->registerNode(DCTOperator::creator(category));
    registry->registerNode(FFTShiftOperator::creator(category));
}

void ImProc::registerAnalysisNodes(std::shared_ptr<NodeRegistry> &registry) const {
    const QString category = "Analysis";
    registry->registerNode(DistanceTransformOperator::creator(category));
    registry->registerNode(ConnectedCompsOperator::creator(category));
}

void ImProc::registerQualityMetricNodes(std::shared_ptr<NodeRegistry> &registry) const {
    const QString category = "Quality";
    registry->registerNode(MseOperator::creator(category));
    registry->registerNode(PsnrOperator::creator(category));
    registry->registerNode(FlipOperator::creator(category));
}

void ImProc::registerRestorationNodes(std::shared_ptr<NodeRegistry> &registry) const {
    const QString category = "Restoration";
    registry->registerNode(DenoiseOperator::creator(category));
}

void ImProc::registerQuantizationNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Quantization";
    registry->registerNode(QuantizeOperator::creator(category));
    registry->registerNode(KMeansOperator::creator(category));
}

} // namespace nitro::ImProc
