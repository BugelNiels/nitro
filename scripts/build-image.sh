#!/bin/bash
# Builds an AppImage from the project

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"

# Build the project
cd ..
mkdir -p build
cd build/ || exit
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=/usr/lib
make -j$(nproc)
make DESTDIR=../appdir -j$(nproc) install

# Navigate to the appdir directory
find ../appdir/
if [[ $? -eq 0 ]]; then
  cd ..
  # Ensure Qt6 is in the path
  export PATH=/usr/lib/qt6/bin/:$PATH

  # Get the linuxdeployqt tool if it does not exist yet
  if [[ ! -f linuxdeployqt-continuous-x86_64.AppImage ]]; then
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod u+x linuxdeployqt-continuous-x86_64.AppImage
  fi
  # TODO save this version elsewhere
  project_version=$(head -n 1 version.txt)
  export VERSION=$project_version
  ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/*.desktop -appimage
  mkdir bin/
  echo "Copying to bin"
  cp NITRO-${project_version}-x86_64.AppImage bin/
  cd "$initial_loc"
else
  cd "$initial_loc"
fi
