#!/bin/bash
# Builds the AppImage in a specific container to ensure that versions are met

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"

cd ..
docker build -t build_image -f docker/build.Dockerfile .
docker run --device=/dev/fuse --cap-add SYS_ADMIN --security-opt apparmor:unconfined build_image
CONTAINER_ID=$(docker ps --latest --quiet)
mkdir -p bin/
echo "Copying AppImage"
docker cp ${CONTAINER_ID}:/app/bin/. bin/
cd scripts || exit
echo "Copied AppImage to bin/"
cd "$initial_loc"
