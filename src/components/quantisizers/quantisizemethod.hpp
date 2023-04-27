#pragma once

#include <QMap>

#include "quantisizers/defaultquantisizer.hpp"
#include "quantisizers/kmeansquantisizer.hpp"
#include "quantisizers/quantisizer.hpp"

namespace nitro {

    enum QuantisizeMethod {
        DEFAULT, K_MEANS
    };

    static const QMap<QuantisizeMethod, std::shared_ptr<Quantisizer>> quantisizers{
            {QuantisizeMethod::DEFAULT, std::shared_ptr<Quantisizer>(new DefaultQuantisizer())},
            {QuantisizeMethod::K_MEANS, std::shared_ptr<Quantisizer>(new KMeansQuantisizer())},
    };

    std::shared_ptr<Quantisizer> getQuantisizer(QuantisizeMethod method);

}  // namespace nitro
