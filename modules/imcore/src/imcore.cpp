#include "imcore.hpp"

#include "include/colimagedata.hpp"
#include <nodes/datatypes/integerdata.hpp>
#include <nodes/datatypes/decimaldata.hpp>

#include "gui/imviewer/imviewer.hpp"
#include "gui/imviewer/imviewdockwidget.hpp"

#include <nodes/noderegistry.hpp>
#include <gui/mainwindow.hpp>

#include "nodes/output/imageviewoperator.hpp"
#include "src/nodes/transform/imresize.hpp"
#include "src/nodes/transform/imflip.hpp"
#include "src/nodes/transform/imrotate.hpp"
#include "nodes/input/imagesourceoperator.hpp"
#include "nodes/input/randomoperator.hpp"
#include "nodes/converter/iminfo.hpp"
#include "nodes/converter/immath.hpp"
#include "nodes/converter/booleanmath.hpp"
#include "nodes/converter/reduction.hpp"
#include "nodes/converter/normalize.hpp"
#include "nodes/converter/invert.hpp"
#include "nodes/converter/rgbtobw.hpp"
#include "nodes/converter/separate.hpp"
#include "nodes/converter/combine.hpp"
#include "nodes/color/immix.hpp"
#include "nodes/color/colorspaceconvert.hpp"
#include "nodes/color/colormap.hpp"
#include "include/grayimagedata.hpp"
#include "nodes/transform/imtranslate.hpp"
#include "nodes/transform/matchsize.hpp"
#include "nodes/output/valueviewoperator.hpp"
#include "nodes/input/rgbinput.hpp"
#include "nodes/converter/maprange.hpp"
#include "nodes/color/uniformconvert.hpp"
#include "nodes/converter/histogram.hpp"

namespace nitro::ImCore {

ImCore::ImCore() {
    imageViewer_ = new ImageViewer(new QGraphicsScene());
}

void ImCore::registerNodes(NodeRegistry *registry) {
    registerInputNodes(registry);
    registerOutputNodes(registry, imageViewer_);
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

void ImCore::registerDocks(MainWindow *window) {
    auto imViewDock = new ImViewDockWidget(imageViewer_, window);
    window->registerDock(imViewDock);
}

void ImCore::registerConvertNodes(NodeRegistry *registry) {
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
    registry->registerNode(HistogramOperator::creator(category));
}



void ImCore::registerTransformNodes(NodeRegistry *registry) {
    const QString category = "Transform";
    registry->registerNode(ResizeOperator::creator(category));
    registry->registerNode(ImFlipOperator::creator(category));
    registry->registerNode(ImRotateOperator::creator(category));
    registry->registerNode(TranslateOperator::creator(category));
    registry->registerNode(MatchSizeOperator::creator(category));
}

void ImCore::registerColorNodes(NodeRegistry *registry) {
    const QString category = "Color";
    registry->registerNode(MixOperator::creator(category));
    registry->registerNode(ConvertOperator::creator(category));
    registry->registerNode(UniformConvertOperator::creator(category));
    registry->registerNode(ColorMapOperator::creator(category));
}


void ImCore::registerInputNodes(NodeRegistry *registry) {
    const QString category = "Input";

    // ------ Image Source Node ------
    registry->registerNode(ImageSourceOperator::creator(category));

    // ------ Decimal Source Node ------
    registry->registerNode([category]() {
        NitroNodeBuilder builder("Value", "ValueSource", category);
        return builder.
                withSourcedOutputValue("Value", 0, 0, 1, BoundMode::UNCHECKED)->
                withIcon("number.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                build();
    });

    // ------ Integer Source Node ------
    registry->registerNode([category]() {
        NitroNodeBuilder builder("Integer", "IntegerSource", category);
        return builder.
                withSourcedOutputInteger("Integer", 128, 0, 255, BoundMode::UNCHECKED)->
                withIcon("number.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                build();
    });
    registry->registerNode(RandomOperator::creator(category));
    registry->registerNode(RgbOperator::creator(category));
}

void ImCore::registerOutputNodes(NodeRegistry *registry, ImageViewer *imageViewer) {
    const QString category = "Output";
    registry->registerNode(ImageViewOperator::creator(category, imageViewer));
    registry->registerNode(ValueViewOperator::creator(category));
}

} // namespace nitro::ImCore
