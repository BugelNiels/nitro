#pragma once

#include "nitromodule.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::Compression {

class Compression : public NitroModule {
public:
    Compression();

    void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) override;

private:
    void registerCompressionNodes(std::shared_ptr<NodeRegistry> &registry);
};

} // namespace nitro::Compression
