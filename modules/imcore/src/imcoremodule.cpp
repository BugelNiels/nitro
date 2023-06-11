#include "imcoremodule.hpp"

#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"

#include "gui/imviewer/imviewer.hpp"
#include "gui/imviewer/imviewdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

#include "nodes/operators/convert/convert.hpp"
#include "nodes/operators/convert/separate.hpp"
#include "nodes/operators/convert/combine.hpp"
#include "nodes/operators/convert/grayscale.hpp"
#include "nodes/operators/convert/immix.hpp"
#include "nodes/operators/convert/immath.hpp"
#include "nodes/operators/image/kmeans.hpp"
#include "nodes/operators/image/flip.hpp"
#include "nodes/operators/image/quantize.hpp"
#include "nodes/operators/output/imageviewalgorithm.hpp"
#include "nodes/operators/filters/threshold.hpp"
#include "nodes/operators/filters/boxfilter.hpp"
#include "nodes/operators/filters/denoise.hpp"
#include "nodes/operators/filters/gaussianblur.hpp"
#include "nodes/operators/filters/bilateralfilter.hpp"

namespace nitro::ImCore {

    ImCoreModule::ImCoreModule() {
        imageViewer_ = new ImageViewer(new QGraphicsScene());
    }

    void ImCoreModule::registerNodes(NodeRegistry *registry) {
        registerInputNodes(registry);
        registerOutputNodes(registry, imageViewer_);
        registerConvertNodes(registry);
        registerImageNodes(registry);
        registerFilterNodes(registry);
    }

    void ImCoreModule::registerDataTypes(NodeRegistry *registry) {
        registry->registerDataType(ImageData::dataInfo());
        registry->registerDataType(IntegerData::dataInfo());
        registry->registerDataType(DecimalData::dataInfo());
    }

    void ImCoreModule::registerDocks(nitro::MainWindow *window) {
        auto imViewDock = new ImViewDockWidget(imageViewer_, window);
        window->registerDock(imViewDock);
    }

    void ImCoreModule::registerConvertNodes(NodeRegistry *registry) {
        const QString category = "Convert";
        registry->registerNode(MixOperator::creator(category));
        registry->registerNode(MathOperator::creator(category));
        registry->registerNode(SeparateOperator::creator(category));
        registry->registerNode(CombineOperator::creator(category));
        registry->registerNode(ConvertOperator::creator(category));
        registry->registerNode(GrayscaleConvert::creator(category));
    }

    void ImCoreModule::registerImageNodes(NodeRegistry *registry) {
        const QString category = "Image";
        registry->registerNode(FlipOperator::creator(category));
        registry->registerNode(QuantizeOperator::creator(category));
        registry->registerNode(KMeansOperator::creator(category));
    }

    void ImCoreModule::registerInputNodes(NodeRegistry *registry) {
        const QString category = "Input";

        // ------ Image Source Node ------
        registry->registerNode([category]() {
            NitroNodeBuilder builder("Image Source", "ImageSource", category);
            return builder.
                    withLoadedOutputImage("Image")->
                    withIcon(":/icons/nodes/image_source.png")->
                    withNodeColor({121, 70, 29})->
                    build();
        });

        // ------ Decimal Source Node ------
        registry->registerNode([category]() {
            NitroNodeBuilder builder("Value", "ValueSource", category);
            return builder.
                    withSourcedOutputValue("Value")->
                    withIcon(":/icons/nodes/number.png")->
                    withNodeColor({131, 49, 74})->
                    build();
        });

        // ------ Integer Source Node ------
        registry->registerNode([category]() {
            NitroNodeBuilder builder("Integer", "IntegerSource", category);
            return builder.
                    withSourcedOutputInteger("Integer")->
                    withIcon(":/icons/nodes/number.png")->
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
