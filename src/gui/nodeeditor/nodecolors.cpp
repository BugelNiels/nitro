#include "nodecolors.h"
#include "src/nodes/input/imagesourcedatamodel.hpp"
#include "src/nodes/output/imageviewerdatamodel.hpp"
#include "src/nodes/conversions/tograyscaledatamodel.hpp"
#include "src/nodes/operators/thresholddatamodel.hpp"
#include "src/nodes/operators/quantization/kmeansdatamodel.hpp"
#include "src/nodes/operators/quantization/quantisizedatamodel.hpp"
#include "src/nodes/operators/flipdatamodel.hpp"
#include "src/nodes/operators/reconstruction/resampledatamodel.hpp"
#include "src/nodes/conversions/seperatergbdatamodel.hpp"

QMap<QString, QColor> nitro::NodeColors::colors;

void nitro::NodeColors::initColors() {
    colors[nitro::ImageSourceDataModel::nodeName()] = {161, 74, 74};
    colors[nitro::ImageViewerDataModel::nodeName()] = {99, 28, 28};

    colors[nitro::ToGrayScaleDataModel::nodeName()] = {59, 90, 115};
    colors[nitro::ThresholdDataModel::nodeName()] = {59, 90, 115};
    colors[nitro::SeperateRgbDataModel::nodeName()] = {59, 90, 115};

    colors[nitro::KMeansDataModel::nodeName()] = {95, 120, 83};
    colors[nitro::QuantisizeDataModel::nodeName()] = {95, 120, 83};

    colors[nitro::FlipDataModel::nodeName()] = {118, 185, 0};

    colors[nitro::ResampleDataModel::nodeName()] = {201, 94, 6};


}

QColor nitro::NodeColors::getColor(const QString &name) {
    if (colors.contains(name)) {
        return colors[name];
    }
    return QColor{168, 105, 49};
}
