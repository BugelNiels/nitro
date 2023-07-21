#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::Compression {

class Compression : public NitroModule {
public:
    Compression();

    void registerNodes(NodeRegistry *registry) override;

private:
    void registerCompressionNodes(NodeRegistry *registry);
};

} // namespace nitro::Compression
