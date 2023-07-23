#pragma once

#include "nitronodebuilder.hpp"
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtNodes/NodeInfo.hpp>

namespace nitro {

/**
 * General registry class containing the information of all the supported nodes.
 */
class NodeRegistry {

public:
    /**
     * Creates a new empty registry.
     */
    NodeRegistry();

    /**
     * This function can be used to register a new node.
     * @param buildFunction A function that returns a pointer to a NitroNode.
     * The function should generate a new node with the desired functionality every time it is called.
     */
    void registerNode(const std::function<std::unique_ptr<NitroNode>()> &buildFunction);

    /**
     * Get all the node categories and the nodes within said category.
     * @return A list of node categories. Each category is stored as a pair, where the first item is the name of the category.
     * The second item is a list containing the information for each node within the category.
     */
    [[nodiscard]] std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> getCategories()
            const;

    /**
     * Get the registry as required by the QtNodes library.
     * @return A registry in the format required by QtNodes.
     */
    [[nodiscard]] const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &getRegistry() const;

private:
    int catIdx_ = 0;
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry_;
    std::unordered_map<QString, std::vector<QtNodes::NodeInfo>> categories_;
    std::unordered_map<QString, int> categoryOrder_;
};

} // namespace nitro
