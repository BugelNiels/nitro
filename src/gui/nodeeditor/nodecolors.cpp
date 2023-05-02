#include "nodecolors.hpp"
#include "src/nodes/input/imagesourcedatamodel.hpp"
#include "src/nodes/output/imageviewerdatamodel.hpp"
#include "src/nodes/conversions/tograyscaledatamodel.hpp"
#include "src/nodes/operators/thresholddatamodel.hpp"
#include "src/nodes/operators/quantization/kmeansdatamodel.hpp"
#include "src/nodes/operators/quantization/quantisizedatamodel.hpp"
#include "src/nodes/operators/flipdatamodel.hpp"
#include "src/nodes/operators/reconstruction/resampledatamodel.hpp"
#include "src/nodes/conversions/rgb/separatergbdatamodel.hpp"
#include "src/nodes/operators/math/imgmathdatamodel.hpp"
#include "src/nodes/conversions/rgb/combinergbdatamodel.hpp"

QMap<QString, QColor> nitro::NodeColors::colors;

void nitro::NodeColors::initColors() {
    colors[nitro::ImageSourceDataModel::nodeName()] = nitro::ImageSourceDataModel::nodeColor();
    colors[nitro::ImageViewerDataModel::nodeName()] = nitro::ImageViewerDataModel::nodeColor();

    colors[nitro::ToGrayScaleDataModel::nodeName()] = nitro::ToGrayScaleDataModel::nodeColor();
    colors[nitro::ThresholdDataModel::nodeName()] = nitro::ThresholdDataModel::nodeColor();
    colors[nitro::SeparateRgbDataModel::nodeName()] = nitro::SeparateRgbDataModel::nodeColor();
    colors[nitro::CombineRgbDataModel::nodeName()] = nitro::CombineRgbDataModel::nodeColor();
    colors[nitro::ImgMathDataModel::nodeName()] = nitro::ImgMathDataModel::nodeColor();

    colors[nitro::KMeansDataModel::nodeName()] = nitro::KMeansDataModel::nodeColor();
    colors[nitro::QuantisizeDataModel::nodeName()] = nitro::QuantisizeDataModel::nodeColor();

    colors[nitro::FlipDataModel::nodeName()] = nitro::FlipDataModel::nodeColor();

    colors[nitro::ResampleDataModel::nodeName()] = nitro::ResampleDataModel::nodeColor();
}

QColor nitro::NodeColors::getColor(const QString &name) {
    if (colors.contains(name)) {
        return colors[name];
    }
    return QColor{168, 105, 49};
}
