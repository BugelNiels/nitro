#pragma once

#include <vector>
#include "nodes/noderegistry.hpp"
#include "mainwindow.hpp"

namespace nitro {

    class NitroModule {
    public:

        virtual ~NitroModule() = default;

        virtual void registerNodes(NodeRegistry* registry) = 0;

        virtual void registerDataTypes(NodeRegistry* registry) {};

        virtual void registerDocks(MainWindow* window) {};
    };

}