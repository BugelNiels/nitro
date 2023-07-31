#include "imcore.hpp"

#include "include/colimagedata.hpp"
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/datatypes/integerdata.hpp>

#include "gui/histogram/histogramdockwidget.hpp"
#include "gui/histogram/histogramviewer.hpp"
#include "gui/imviewer/imageviewer.hpp"
#include "gui/imviewer/imviewdockwidget.hpp"

#include <gui/mainwindow.hpp>
#include <nodes/noderegistry.hpp>

#include "include/grayimagedata.hpp"
#include "nodes/color/colormap.hpp"
#include "nodes/color/colorspaceconvert.hpp"
#include "nodes/color/immix.hpp"
#include "nodes/color/uniformconvert.hpp"
#include "nodes/converter/booleanmath.hpp"
#include "nodes/converter/combine.hpp"
#include "nodes/converter/iminfo.hpp"
#include "nodes/converter/immath.hpp"
#include "nodes/converter/invert.hpp"
#include "nodes/converter/maprange.hpp"
#include "nodes/converter/normalize.hpp"
#include "nodes/converter/reduction.hpp"
#include "nodes/converter/rgbtobw.hpp"
#include "nodes/converter/separate.hpp"
#include "nodes/input/imagesourceoperator.hpp"
#include "nodes/input/randomoperator.hpp"
#include "nodes/input/rgbinput.hpp"
#include "nodes/output/imageviewoperator.hpp"
#include "nodes/output/valueviewoperator.hpp"
#include "nodes/transform/imtranslate.hpp"
#include "nodes/transform/matchsize.hpp"
#include "src/nodes/output/histogramviewoperator.hpp"
#include "src/nodes/transform/imflip.hpp"
#include "src/nodes/transform/imresize.hpp"
#include "src/nodes/transform/imrotate.hpp"


namespace nitro::ImCore {

ImCore::ImCore() {}

void ImCore::registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) {
    window_ = window;
    registerInputNodes(registry);
    registerOutputNodes(registry);
    registerConvertNodes(registry);
    registerColorNodes(registry);
    registerTransformNodes(registry);
}

void ImCore::registerDataTypes() {
    ColImageData::registerConversions();
    GrayImageData::registerConversions();
    DecimalData::registerConversions();
    IntegerData::registerConversions();
}

void ImCore::registerConvertNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Converter";
    registry->registerNode(ImInfoOperator::creator(category)); // TODO: place elsewhere?
    registry->registerNode(MathOperator::creator(category));
    registry->registerNode(BooleanMathOperator::creator(category));
    registry->registerNode(ReductionOperator::creator(category));
    registry->registerNode(MapRangeOperator::creator(category));
    registry->registerNode(NormalizeOperator::creator(category));
    registry->registerNode(InvertOperator::creator(category));
    registry->registerNode(RgbToGrayscaleOperator::creator(category));
    registry->registerNode(SeparateOperator::creator(category));
    registry->registerNode(CombineOperator::creator(category));
}

void ImCore::registerTransformNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Transform";
    registry->registerNode(ResizeOperator::creator(category));
    registry->registerNode(ImFlipOperator::creator(category));
    registry->registerNode(ImRotateOperator::creator(category));
    registry->registerNode(TranslateOperator::creator(category));
    registry->registerNode(MatchSizeOperator::creator(category));
}

void ImCore::registerColorNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Color";
    registry->registerNode(MixOperator::creator(category));
    registry->registerNode(ConvertOperator::creator(category));
    registry->registerNode(UniformConvertOperator::creator(category));
    registry->registerNode(ColorMapOperator::creator(category));
}

void ImCore::registerInputNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Input";

    // ------ Image Source Node ------
    registry->registerNode(ImageSourceOperator::creator(category));

    // ------ Decimal Source Node ------
    registry->registerNode([category]() {
        NitroNodeBuilder builder("Value", "ValueSource", category);
        return builder.withSourcedOutputValue("Value", 0, 0, 1, BoundMode::UNCHECKED)
                ->withIcon("number.png")
                ->withNodeColor(NITRO_INPUT_COLOR)
                ->build();
    });

    // ------ Integer Source Node ------
    registry->registerNode([category]() {
        NitroNodeBuilder builder("Integer", "IntegerSource", category);
        return builder.withSourcedOutputInteger("Integer", 128, 0, 255, BoundMode::UNCHECKED)
                ->withIcon("number.png")
                ->withNodeColor(NITRO_INPUT_COLOR)
                ->build();
    });
    registry->registerNode(RandomOperator::creator(category));
    registry->registerNode(RgbOperator::creator(category));
}

void ImCore::registerOutputNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Output";
    registry->registerNode(ImageViewOperator::creator(category, window_));
    registry->registerNode(ValueViewOperator::creator(category));
    registry->registerNode(HistogramViewOperator::creator(category, window_));
}

} // namespace nitro::ImCore
