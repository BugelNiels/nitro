#include "imcoremodule.hpp"

#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"

#include "gui/imviewer/imviewer.hpp"
#include "gui/imviewer/imviewdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

#include "nodes/convert/convert.hpp"
#include "nodes/convert/separate.hpp"
#include "nodes/convert/combine.hpp"
#include "nodes/convert/grayscale.hpp"
#include "nodes/convert/immix.hpp"
#include "nodes/convert/immath.hpp"
#include "nodes/quantization//kmeans.hpp"
#include "src/nodes/comparison/flip.hpp"
#include "nodes/quantization//quantize.hpp"
#include "nodes/output/imageviewoperator.hpp"
#include "nodes/filters/threshold.hpp"
#include "nodes/filters/boxfilter.hpp"
#include "nodes/filters/denoise.hpp"
#include "nodes/filters/gaussianblur.hpp"
#include "nodes/filters/bilateralfilter.hpp"
#include "src/nodes/transform/imresize.hpp"
#include "src/nodes/util/iminfo.hpp"
#include "src/nodes/transform/imflip.hpp"
#include "src/nodes/transform/imrotate.hpp"
#include "nodes/util/normalize.hpp"
#include "nodes/util/invert.hpp"
#include "nodes/util/colormap.hpp"
#include "nodes/input/imagesourceoperator.hpp"

namespace nitro::ImCore {

    ImCoreModule::ImCoreModule() {
        imageViewer_ = new ImageViewer(new QGraphicsScene());
    }

    void ImCoreModule::registerNodes(NodeRegistry *registry) {
        registerInputNodes(registry);
        registerOutputNodes(registry, imageViewer_);
        registerConvertNodes(registry);
        registerTransformNodes(registry);
        registerQuantizationNodes(registry);
        registerFilterNodes(registry);
        registerComparisonNodes(registry);
        registerUtilNodes(registry);
    }

    void ImCoreModule::registerDataTypes(NodeRegistry *registry) {
        ImageData::registerConversion(DecimalData::dataInfo(), [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
            auto imData = std::static_pointer_cast<DecimalData>(nodeData);
            double val = imData->data();
            return std::make_shared<cv::Mat>(1, 1, CV_32F, cv::Scalar(val));
        });
    }

    void ImCoreModule::registerDocks(nitro::MainWindow *window) {
        auto imViewDock = new ImViewDockWidget(imageViewer_, window);
        window->registerDock(imViewDock);
    }

    void ImCoreModule::registerConvertNodes(NodeRegistry *registry) {
        const QString category = "Convert";
        registry->registerNode(MathOperator::creator(category));
        registry->registerNode(MixOperator::creator(category));
        registry->registerNode(ConvertOperator::creator(category));
        registry->registerNode(GrayscaleConvertOperator::creator(category));
        registry->registerNode(SeparateOperator::creator(category));
        registry->registerNode(CombineOperator::creator(category));
    }

    void ImCoreModule::registerQuantizationNodes(NodeRegistry *registry) {
        const QString category = "Quantization";
        registry->registerNode(FlipOperator::creator(category));
        registry->registerNode(QuantizeOperator::creator(category));
        registry->registerNode(KMeansOperator::creator(category));
    }

    void ImCoreModule::registerTransformNodes(NodeRegistry *registry) {
        const QString category = "Transform";
        registry->registerNode(ResizeOperator::creator(category));
        registry->registerNode(ImFlipOperator::creator(category));
        registry->registerNode(ImRotateOperator::creator(category));
    }

    void ImCoreModule::registerUtilNodes(NodeRegistry *registry) {
        const QString category = "Util";
        registry->registerNode(ImInfoOperator::creator(category));
        registry->registerNode(NormalizeOperator::creator(category));
        registry->registerNode(InvertOperator::creator(category));
        registry->registerNode(ColorMapOperator::creator(category));
    }

    void ImCoreModule::registerComparisonNodes(NodeRegistry *registry) {
        const QString category = "Compare";
        registry->registerNode(FlipOperator::creator(category));
    }

    void ImCoreModule::registerInputNodes(NodeRegistry *registry) {
        const QString category = "Input";

        // ------ Image Source Node ------
        registry->registerNode(ImageSourceOperator::creator(category));

        // ------ Decimal Source Node ------
        registry->registerNode([category]() {
            NitroNodeBuilder builder("Value", "ValueSource", category);
            return builder.
                    withSourcedOutputValue("Value", 0, 0, 1, BoundMode::UNCHECKED)->
                    withIcon("number.png")->
                    withNodeColor({131, 49, 74})->
                    build();
        });

        // ------ Integer Source Node ------
        registry->registerNode([category]() {
            NitroNodeBuilder builder("Integer", "IntegerSource", category);
            return builder.
                    withSourcedOutputInteger("Integer", 128, 0, 255, BoundMode::UNCHECKED)->
                    withIcon("number.png")->
                    withNodeColor({131, 49, 74})->
                    build();
        });
    }

    void ImCoreModule::registerOutputNodes(NodeRegistry *registry, ImageViewer *imageViewer) {
        const QString category = "Output";
        registry->registerNode(ImageViewAlgorithm::creator(category, imageViewer));
    }

    void ImCoreModule::registerFilterNodes(NodeRegistry *registry) {
        const QString category = "Filter";
        registry->registerNode(ThresholdOperator::creator(category));
        registry->registerNode(BoxFilterOperator::creator(category));
        registry->registerNode(GaussianBlurOperator::creator(category));
        registry->registerNode(BilateralFilterOperator::creator(category));
        registry->registerNode(DenoiseOperator::creator(category));
    }

} // imCore
