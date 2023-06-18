#!/bin/bash

# Builds the project

cd ..
mkdir -p build
cd build/ || exit
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
