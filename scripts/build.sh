#!/bin/bash

# Builds the project

cd ..
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
