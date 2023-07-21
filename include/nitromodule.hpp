#pragma once

#include <vector>

namespace nitro {

class MainWindow;

class NodeRegistry;

/**
 * Defines a generic module interface that any newly created module should inherit from.
 * Nodes and data types can be added by registering them to the NodeRegistry.
 */
class NitroModule {
public:

    /**
     * Default deconstructor.
     */
    virtual ~NitroModule() = default;

    /**
     * Used to register new nodes to the existing registry. If a module introduces new nodes, this function should be implemented.
     * @see NodeRegistry.
     * @param registry Registry.
     */
    virtual void registerNodes(NodeRegistry *registry) {
        // No nodes are registered by default
    }

    /**
     * Used to register new data types. If a module introduces new data types, this function should be implemented.
     * Note that the conversion registration is done via the static methods in the corresponding data classes.
     */
    virtual void registerDataTypes() {
        // No data types are registered by default
    };

    /**
     * Used to register new GUI widgets. This adds the widgets to main window.
     * @param window The window to register the GUI widgets to.
     */
    virtual void registerDocks(MainWindow *window) {
        // No docks are registered by default
    };
};

} // namespace nitro
