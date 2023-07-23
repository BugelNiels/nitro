#!/bin/bash

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"
cd ..

# Builds the project
mkdir -p build
cd build/ || exit
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
if [[ $? -eq 0 ]]; then
  echo "Build success."
  echo ""
  echo -e "\tYou can find the generated binary \"nitro\" in the \"bin/\" directory."
  echo ""
fi
cd "$initial_loc"
