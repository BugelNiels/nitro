#!/bin/bash

cd ..
docker build -t build_image -f docker/build.Dockerfile .
docker run --device=/dev/fuse --cap-add SYS_ADMIN --security-opt apparmor:unconfined build_image
CONTAINER_ID=$(docker ps --latest --quiet)
mkdir -p bin/
echo "Copying AppImage"
docker cp ${CONTAINER_ID}:/app/bin/. bin/
cd scripts || exit
echo "Copied AppImage to bin/"
