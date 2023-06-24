## Project Structure

The project was developed with the aim of making it easily extendable. Below a number of code details are outlined to
illustrate some basic example usages of the project and how to extend it.

### Modules

NITRO works with a module system for its node. By using this module system, users can determine at compile-time which
modules to include and which modules to exclude. These modules are then automatically registered when the program
starts.

The different modules can be found in the [modules/](../modules) directory. Currently, there are two modules here:

- **ImCore**. This is the main module. It provides support for basic image operations and the image viewer
  widget.
- **ImExtra**. Contains additional image processing routines such as morphological operators and frequency domain
  related operators.
- **Im3D**. This module contains some nodes that can be used to compress and enhance bit-depth. Additionally, it
  contains a widget that allows for viewing of images as 3D objects:

Adding new modules can be done by following a similar structure to the existing module:

- Create a new directory in the `modules` directory. The folder structure should be all lower-case.
- Create an implementation of `imcoremodule.hpp`. This implementation should register the new nodes, in addition to the
  data types and widgets (if there are any).
- Create a `CMakeLists.txt` to create a library of the module. See
  e.g. [this CMakeLists.txt](../modules/imcore/CMakeLists.txt) on how to do this.
  Note that any libraries used in the module, must unfortunately still be found and linked in the
  main [CMakeLists.txt](../CMakeLists.txt). A future release should hopefully fix this issue.
- Add any nodes, data types or widgets as needed (see below).

The basic directory structure will then look as follows:
```shell
└── modules
    ├── im3d
    ├── imcore
    ├── imextra
    └── <new module>
        ├── CMakeLists.txt
        ├── include
        │   └── <my module>module.hpp
        └── src
            ├── <my module>module.cpp
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

[Header file](../modules/imcore/src/nodes/filters/bilateralfilter.hpp)

```c++
#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    /**
     * Node that performs Bilateral filtering on an input image.
     */
    class BilateralFilterOperator : public NodeOperator {
    public:
        /**
         * Responsible for creating a function that builds this node.
         *
         * @param category Category name this node should be put in.
         * @return A function that creates this particular node.
         */
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        /**
         * Executes the Bilateral filtering algorithm of this node on the current input data.
         * @param nodePorts Port data containing the current input and output information.
         * @param options Options for passing additional parameters to the algorithm. Currently unused.
         */
        void execute(NodePorts &nodePorts) override;

    };
} // nitro

```

[Source file](../modules/imcore/src/nodes/filters/bilateralfilter.cpp)

```c++
#include "bilateralfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIGMA_C "Sigma (c)"
#define INPUT_SIGMA_S "Sigma (s)"
#define INPUT_D "Diameter"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts) {
    // Get the input data
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);;
    double sigmaCol = nodePorts.inputValue(INPUT_SIGMA_C);
    double sigmaSpace = nodePorts.inputValue(INPUT_SIGMA_S);
    int d = nodePorts.inputInteger(INPUT_D);

    // Perform filtering
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);

    // Store the result
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BilateralFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_D, 9, 1, 64)->
                withInputValue(INPUT_SIGMA_C, 75, 2, 255)->
                withInputValue(INPUT_SIGMA_S, 75, 2, 255)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
```

### Data Types

New data types can be added by creating an implementation of `QtNodes::NodeData`. For example,
the [integer data type](../include/nodes/datatypes/integerdata.hpp) has the following structure:

```c++
namespace nitro {
    class IntegerData : public QtNodes::NodeData {
    public:
        IntegerData() = default;

        explicit IntegerData(int val) : data_(data) {}

        static nitro::DataInfo dataInfo() {
            return {"Value", "integer", {89, 140, 92}};
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        int value() const { return data_; }

    private:
        int data_;
    };
} // nitro
```

Note that, while not required, each data type should ideally have a method for retrieving its data info. This way, the
data type can be registered properly.
