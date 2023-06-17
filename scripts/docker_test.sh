#!/bin/bash

# Runs a docker images with the newly created AppImage to verify it works
# Note that it may not fully work with the screen, but as long as it doesn't give errors, we consider it great success

cd ..
docker build -t test_image -f docker/Dockerfile .
xhost + local:docker
docker run --rm -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -h $(hostname) test_image timeout 1s ./squashfs-root/AppRun
xhost - local:docker
cd scripts || exit
