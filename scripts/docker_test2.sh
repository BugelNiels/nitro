#!/bin/bash

# Runs a docker images with the newly created AppImage to verify it works
# Note that it may not fully work with the screen, but as long as it doesn't give errors, we consider it great success

cd ..
docker build -t build_image -f docker/build.Dockerfile .
xhost + local:docker
docker run -it --rm -device /dev/fuse --cap-add SYS_ADMIN build_image
xhost - local:docker
cd scripts || exit
