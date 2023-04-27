#include "quantisizemethod.hpp"

std::shared_ptr<nitro::Quantisizer> nitro::getQuantisizer(nitro::QuantisizeMethod method) {
    return nitro::quantisizers[method];
}
