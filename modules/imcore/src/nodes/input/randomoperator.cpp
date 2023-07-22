#include "randomoperator.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"
#include <nodes/datatypes/decimaldata.hpp>

#include <opencv2/imgproc.hpp>
#include <random>

namespace nitro::ImCore {

static inline const QString INPUT_SEED = "Seed";
static inline const QString OUTPUT_VAL = "Value";

static double generateRandomDouble(int seed) {
    std::default_random_engine generator(seed);
    std::uniform_real_distribution distribution(0.0, 1.0);
    return distribution(generator);
}

void RandomOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int seed = nodePorts.inputInteger(INPUT_SEED);

    nodePorts.output<DecimalData>(OUTPUT_VAL, generateRandomDouble(seed));
}

std::function<std::unique_ptr<NitroNode>()> RandomOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Random", "random", category);
        return builder.
                withOperator(std::make_unique<RandomOperator>())->
                withIcon("number.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withInputInteger(INPUT_SEED, 0, 0, 100, BoundMode::LOWER_ONLY)->
                withOutputValue(OUTPUT_VAL)->
                build();
    };
}

} // namespace nitro::ImCore
