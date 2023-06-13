#pragma once

#include <vector>

namespace nitro {

    class MainWindow;

    class NodeRegistry;

    class NitroModule {
    public:

        virtual ~NitroModule() = default;

        virtual void registerNodes(NodeRegistry *registry) = 0;

        virtual void registerDataTypes(NodeRegistry *registry) {
            // No data types are registered by default
        };

        virtual void registerDocks(MainWindow *window) {
            // No docks are registered by default
        };
    };

}
