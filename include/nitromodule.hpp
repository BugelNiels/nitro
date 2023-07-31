#pragma once

#include <memory>
#include <vector>

namespace nitro {

class MainWindow;

class NodeRegistry;

/**
 * @brief Defines a generic module interface that any newly created module should inherit from.
 * Nodes and data types can be added by registering them to the NodeRegistry.
 */
class NitroModule {
public:
    /**
     * @brief Default destructor.
     */
    virtual ~NitroModule() = default;

    /**
     * @brief Used to register new nodes to the existing registry. If a module introduces new nodes, this function should be implemented.
     * @see NodeRegistry.
     * @param registry Registry.
     * @param window The window to register the GUI widgets to. Can be used by viewer widgets.
     */
    virtual void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) {
        // No nodes are registered by default
    }

    /**
     * @brief Used to register new data types. If a module introduces new data types, this function should be implemented.
     * Note that the conversion registration is done via the static methods in the corresponding data classes.
     */
    virtual void registerDataTypes(){
            // No data types are registered by default
    };
};

} // namespace nitro
