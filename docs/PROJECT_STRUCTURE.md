## Project Structure

The project was developed with the aim of making it easily extendable. Below a number of code details are outlined to
illustrate some basic example usages of the project and how to extend it.

### Modules

NITRO works with a module system for its node. By using this module system, users can determine at compile-time which
modules to include and which modules to exclude. These modules are then automatically registered when the program
starts.

The different modules can be found in the [modules/](../modules) directory. Currently, there are two modules here:

- **ImCore**. This is the main module. It provides support for basic image and number operations and the image viewer
  widget.
- **ImProc**. Contains a significant number of image processing routines such as morphological operators and frequency domain
  related operators.
- **Compression**. This module contains some nodes that can be used to compress images.
- **Thesis**. This module contains the nodes relevant for Niels' Master thesis (disabled by default). Includes the 3D viewer.

Adding new modules can be done by following a similar structure to the existing module:

- Create a new directory in the `modules` directory. The folder structure should be all lower-case.
- Create an implementation of `imcoremodule.hpp`. This implementation should register the new nodes, in addition to the
  data types and widgets (if there are any).
- Create a `CMakeLists.txt` to create a library of the module. See
  e.g. [this CMakeLists.txt](../modules/imcore/CMakeLists.txt) on how to do this.
  Note that some libraries used in the module, must unfortunately still be found and linked in the
  main [CMakeLists.txt](../CMakeLists.txt). A future release should hopefully fix this issue.
- Add any nodes, data types or widgets as needed (see below).

The basic directory structure will then look as follows:

```shell
└── modules
    ├── compression
    ├── imcore
    ├── improc
    ├── thesis
    └── <new module>
        ├── CMakeLists.txt
        ├── include
        │   └── <my module>.hpp
        └── src
            ├── <my module>.cpp
            ├── gui       # Dock widgets go here
            └── nodes     # Node implementation go here
```

### Nodes

Nodes are built using the builder-pattern to ensure maximum flexibility. As such, new nodes are not created by creating
a subclass of `NitroNode`. Instead, nodes are built dynamically at runtime.
Each node will typically have a `NodeOperator`. This operator is what performs the actual computation of the node.
For details on how to construct a node, you can look at the [NitroNodeBuilder](../include/nodes/nitronodebuilder.hpp)
class.

A simple implementation of a denoising node is then as follows:

[Header file](../modules/improc/src/nodes/blur/bilateralfilter.hpp)

```c++
#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImProc {

/**
 * Node that performs Bilateral filtering on an input image.
 */
class BilateralFilterOperator : public NodeOperator {
public:
    /**
     * @brief Responsible for creating a function that builds this node.
     *
     * @param category Category name this node should be put in.
     * @return A function that creates this particular node.
     */
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    /**
     * @brief Executes the Bilateral filtering algorithm of this node on the current input data.
     * @param nodePorts Port data containing the current input and output information.
     * @param options Options for passing additional parameters to the algorithm. Currently unused.
     */
    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImProc

```

[Source file](../modules/improc/src/nodes/blur/bilateralfilter.cpp)

```c++
#include "bilateralfilter.hpp"
#include <colimagedata.hpp>
#include <nodes/nitronodebuilder.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_SIGMA_C = "Sigma (c)";
static inline const QString INPUT_SIGMA_S = "Sigma (s)";
static inline const QString INPUT_D = "Diameter";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";

void BilateralFilterOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    double sigmaCol = nodePorts.inputValue(INPUT_SIGMA_C);
    double sigmaSpace = nodePorts.inputValue(INPUT_SIGMA_S);
    int d = nodePorts.inputInteger(INPUT_D);

    // Perform filtering
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);

    // Store the result
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> BilateralFilterOperator::creator(
        const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.withOperator(std::make_unique<BilateralFilterOperator>())
                ->withIcon("blur.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_D, 9, 1, 64)
                ->withInputValue(INPUT_SIGMA_C, 75, 2, 255)
                ->withInputValue(INPUT_SIGMA_S, 75, 2, 255)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImProc
```

### Data Types

New data types can be added by creating an implementation of `QtNodes::NodeData`. For example,
the [integer data type](../include/nodes/datatypes/integerdata.hpp) has the following structure:

[Header file](../include/nodes/datatypes/integerdata.hpp)

```c++
#pragma once

#include <utility>

#include "flexibledata.hpp"
#include <QtNodes/NodeData>

namespace nitro {

/**
 * @brief Describes an integer data type.
 */
class IntegerData : public FlexibleData<int, IntegerData> {
public:
    /**
     * @brief Creates a new empty integer data type.
     */
    IntegerData();

    /**
     * @brief Creates a integer data type with the provided value.
     * @param value The value this type should have.
     */
    explicit IntegerData(int value);

    /**
     * @brief Returns the unique id of this data type.
     * @return The unique id of this data type.
     */
    static QString id() { return id_; }

    /**
     * @brief Registers the conversions that this data type allows.
     */
    static void registerConversions();

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of this data type.
     */
    [[nodiscard]] QString getDescription() const override;

private:
    inline static const QString name_ = "Integer";
    inline static const QString id_ = "Integer";
    inline static const QColor baseColor_ = {89, 140, 92};
};

} // namespace nitro
```

[Source file](../src/core/nodes/datatypes/integerdata.cpp)

```c++
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/datatypes/integerdata.hpp>

namespace nitro {

IntegerData::IntegerData() : FlexibleData<int, IntegerData>(0, id_, name_, baseColor_) {
    allowConversionFrom(DecimalData::id());
}

IntegerData::IntegerData(int value)
    : FlexibleData<int, IntegerData>(value, id_, name_, baseColor_) {
    allowConversionFrom(DecimalData::id());
}

QString IntegerData::getDescription() const {
    return QString::number(data());
}

void IntegerData::registerConversions() {

    // Every type needs a "conversion" to itself
    IntegerData::registerConversionFrom<IntegerData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                return std::static_pointer_cast<IntegerData>(nodeData)->data();
            });

    IntegerData::registerConversionFrom<DecimalData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                auto doubleData = std::static_pointer_cast<DecimalData>(nodeData);
                return int(std::round(doubleData->data()));
            });
}

} // namespace nitro
```

Data types allow for custom conversions. These are defined in the `registerConversions()` function. Note that not all
the conversions. By default, only the conversions registered in the constructor are enabled. If more conversions should
be supported, then these should be specified in the node builder (see
e.g. [immath.cpp](../modules/imcore/src/nodes/converter/immath.cpp)).
