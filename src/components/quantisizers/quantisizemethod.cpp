#include "quantisizemethod.hpp"

std::shared_ptr<nitro::Quantisizer> getQuantisizer(nitro::QuantisizeMethod method) {
    return nitro::quantisizers[method];
}
