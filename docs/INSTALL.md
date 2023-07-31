# Installing

These instructions will get you a copy of the project up and running on your local machine. Note that these instructions
are intended for building the project from source code. If you are simply interested in running the application, have a
look at the latest release instead.

## 1. Downloading the Repository

To set up the repository locally, clone the repository:

```shell
git clone --recurse-submodules https://github.com/BugelNiels/nitro.git
```

In case `--recurse-submodules` was not used, you can initialize the submodules using:

```shell
git submodule init
git submodule update
```

Note, if you are a developer on the project, you probably want to clone everything using `ssh` instead of `https`.
To do this, execute:

```shell
git config url."ssh://git@".insteadOf https://
```

before cloning.

## 2. Prerequisites

- [CMake 3.9+](https://cmake.org/)
- [Qt6](https://www.qt.io/product/qt6)
- [OpenCV 4](https://opencv.org/)
- [OpenGL 4.1+](https://www.opengl.org/)

### 2.1 Install Instructions

Note that a plain [Ubuntu docker image](../docker/ubuntu.Dockerfile) can be found in the [docker/](../docker) directory.

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

### 2.2 Dependencies

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

## 3. Building & Running

To build the project, can execute the `build.sh` script in the `scripts/` directory and run the resulting binary:

```shell
cd scripts
bash build.sh
cd ..
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
