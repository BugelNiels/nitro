<br />
<p align="center">
  <h1 align="center">NITRO</h1>

  <p align="center">
    Powerful and flexible node editor that facilitates building complex combinations of various image processing techniques. 
  </p>
</p>

![stars](https://img.shields.io/github/stars/bugelniels/nitro.svg) ![watchers](https://img.shields.io/github/watchers/bugelniels/nitro.svg) ![license](https://img.shields.io/github/license/bugelniels/nitro.svg?cachebust=1) ![follow](https://img.shields.io/github/followers/bugelniels.svg?style=social&label=Follow&maxAge=2592000)

## About the project

NITRO is a powerful tool for building complex image processing routines. Instead of needing to modify source code or
scripts to process images in different ways, you can use this visual node editor to do that instead. The project was
developed with extendability in mind, making it easy to add your own types nodes, and widgets. For more information on
this, see [Project Structure](#project-structure). The UI was inspired by
Blender's [Shader Editor](https://docs.blender.org/manual/en/latest/editors/shader_editor.html)

The project was originally developed for my Master thesis at the University of Groningen. That is also the origin of the
name NITRO: Niels' Image-processing Thesis on Reconstruction Operations

![Node Editor](screenshots/demo.gif)

## Disclaimer

The project is still in development and can be considered in early-alpha. As such, I am still working hard on fixing
bugs and adding new features.
If you encounter any, please feel free to create an issue.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### 1. Prerequisites

- [CMake 3.9+](https://cmake.org/)
- [Qt6](https://www.qt.io/product/qt6)
- [OpenGL 4.1+](https://www.opengl.org/)
- [OpenCV 4](https://opencv.org/)

#### 1.1 Install Instructions

**Ubuntu:**

- CMake
  ```shell
  sudo apt install cmake
  ```
- Qt6
  ```shell
  sudo apt install qt6-base-dev
  ```
- OpenCV 4
  ```shell
  sudo apt install libopencv-dev
  ```
- OpenGL.
  Should be supported by default. Might need to install `mesa-utils`
  ```shell
  sudo apt install mesa-utils
  ```

**Mac:**

- CMake
  ```shell
  brew install cmake
  ```
- Qt6
  ```shell
  brew install qt
  ```
- OpenCV 4
  ```shell
  brew install opencv
  ```

> If on Windows, good luck.

#### 1.2 Dependencies

These dependencies are used by NITRO and are included as Git submodules. As such, these don't need to be manually
installed.

- [QtNodes](https://github.com/BugelNiels/nodeeditor)
    - A custom fork of the [QtNodes](https://github.com/paceholder/nodeeditor) repo. This fork was specifically created
      to work with NITRO.
- [Qt Value Sliders](https://github.com/BugelNiels/qt-value-slider)
    - A custom widget that allows for user-friendly control of numerical inputs. Resembles the widgets used in Blender
      to select numerical values.
- [FLIP](https://github.com/NVlabs/flip)
    - A state-of-the-art image comparison algorithm developed by NVIDIA.
- [Spline](https://github.com/ttk592/spline/)
    - Small library that provides cubic spline interpolation functionality.

### 2. Installing

To set up the repository locally, clone the repository:

```shell
git clone --recurse-submodules git@github.com:BugelNiels/nitro.git
```

In case `--recurse-submodules` was not used, you can initialize the submodules using:

```shell
git submodule init
git submodule update
```

### 3. Building & Running

To build the project, can execute the `build.sh` script and run the resulting binary:

```shellell
sh build.sh
./bin/nitro
```

Alternatively, you can manually build and run the program:

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
./nitro
```

## Project Structure

The project was developed with the aim of making it easily extendable. Below a number of code details are outlined to
illustrate some basic example usages of the project and how to extend it.

### Modules

NITRO works with a module system for its node. By using this module system, users can determine at compile-time which
modules to include and which modules to exclude. These modules are then automatically registered when the program
starts.

The different modules can be found in the `modules/` directory. Currently, there are two modules here:

- **imcore**. This is the main module. It provides support for basic image operations and the image viewer
  widget: ![Image Viewer](screenshots/imviewer.gif)
- **im3d**. This module contains some nodes that can be used to compress and enhance bit-depth. Additionally, it
  contains a widget that allows for viewing of images as 3D objects: ![3D Viewer](screenshots/3dviewer.gif)

Adding new modules can be done by following a similar structure to the existing module:

- Create an implementation of `imcoremodule.hpp`. This implementation should register the new nodes, in addition to the
  data types and widgets (if there are any).
- Create a `CMakeLists.txt` to create a library of the module. See
  e.g. [this CMakeLists.txt](modules/imcore/CMakeLists.txt) on how to do this.
  Note that any libraries used in the module, must unfortunately still be found and linked in the
  main [CMakeLists.txt](CMakeLists.txt). A future release should hopefully fix this issue.
- Add any nodes, data types or widgets as needed (see below).

### Nodes

Nodes are built using the builder-pattern to ensure maximum flexibility. As such, new nodes are not created by creating
a subclass of `NitroNode`. Instead, nodes are built dynamically at runtime.
Each node will typically have a `NodeOperator`. This operator is what performs the actual computation of the node.
For details on how to construct a node, you can look at the [NitroNodeBuilder](include/nodes/nitronodebuilder.hpp)
class.

A simple implementation of a denoising node is then as follows:

[Header file](modules/imcore/src/nodes/operators/filters/bilateralfilter.hpp)

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
        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    };
} // nitro

```

[Source file](modules/imcore/src/nodes/operators/filters/bilateralfilter.cpp)

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

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    // Verifying that all the required inputs are there
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_SIGMA_C, INPUT_SIGMA_S, INPUT_D})) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    double sigmaCol = nodePorts.getInputValue(INPUT_SIGMA_C);
    double sigmaSpace = nodePorts.getInputValue(INPUT_SIGMA_S);
    int d = nodePorts.getInputInteger(INPUT_D);

    // Perform filtering
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);

    // Store the result
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BilateralFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_D, 9, 1, 64)->
                withInputValue(INPUT_SIGMA_C, 75, 2, 255)->
                withInputValue(INPUT_SIGMA_S, 75, 2, 255)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
```

### Data Types

New data types can be added by creating an implementation of `QtNodes::NodeData`. For example,
the [integer data type](include/nodes/datatypes/integerdata.hpp) has the following structure:

```c++
namespace nitro {
    class IntegerData : public QtNodes::NodeData {
    public:
        IntegerData() = default;

        explicit IntegerData(int val) : val_(val) {}

        static QtNodes::DataInfo dataInfo() {
            return {"Value", "integer", {89, 140, 92}};
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        int value() const { return val_; }

    private:
        int val_;
    };
} // nitro
```

Note that, while not required, each data type should ideally have a method for retrieving its data info. This way, the
data type can be registered properly.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

