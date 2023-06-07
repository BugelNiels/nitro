#include "nitronodes.hpp"
#include "src/core/nodes/operators/output/imageviewalgorithm.hpp"
#include "src/core/nodes/operators/output/surfaceviewalgorithm.hpp"


std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildOutputNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Output";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    // ------ Image Viewer Node ------
    auto imViewNode = [category, this]() {
        nitro::NitroNodeBuilder builder("Image Viewer", "ImageViewer", category,
                                        std::make_unique<nitro::ImageViewAlgorithm>(imViewer_));
        auto node = builder.
                withInputGreyImage("image")->
                withIcon(":/icons/nodes/viewer.png")->
                withNodeColor({60, 29, 38})->
                build();
        return node;
    };
    nodeInfos.emplace_back(imViewNode()->getInfo());
    creators.emplace_back(std::move(imViewNode));

    // ------ Surface Viewer Node ------
    auto surfViewNode = [category, this]() {
        nitro::NitroNodeBuilder builder("3D Image Viewer", "ImageViewer3D", category,
                                        std::make_unique<nitro::SurfaceViewAlgorithm>(surfViewer_));
        auto node = builder.
                withInputGreyImage("image")->
                withIcon(":/icons/nodes/surface.png")->
                withNodeColor({99, 28, 28})->
                build();
        return node;
    };
    nodeInfos.emplace_back(surfViewNode()->getInfo());
    creators.emplace_back(std::move(surfViewNode));

    return {category, nodeInfos};
}