#pragma once

#include <vector>

namespace nitro {

    class MainWindow;

    class NodeRegistry;

    /**
     *
     */
    class NitroModule {
    public:

        /**
         *
         */
        virtual ~NitroModule() = default;

        /**
         *
         * @param registry
         */
        virtual void registerNodes(NodeRegistry *registry) = 0;

        /**
         *
         * @param registry
         */
        virtual void registerDataTypes(NodeRegistry *registry) {
            // No data types are registered by default
        };

        /**
         *
         * @param window
         */
        virtual void registerDocks(MainWindow *window) {
            // No docks are registered by default
        };
    };

}
