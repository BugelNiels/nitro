#!/bin/bash

# Builds an AppImage from the project

cd ..
mkdir -p build
cd build/ || exit
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=/usr/lib
make -j$(nproc)
make DESTDIR=../appdir -j$(nproc) install
find ../appdir/
if [[ $? -eq 0 ]]; then
  cd ..
  export PATH=/usr/lib/qt6/bin/:$PATH

  if [[ ! -f linuxdeployqt-continuous-x86_64.AppImage ]]; then
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod u+x linuxdeployqt-continuous-x86_64.AppImage
  fi
  # TODO save this version elsewhere
  export VERSION="1.3.1"
  ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/*.desktop -appimage
  mkdir bin/
  echo "Copying to bin"
  cp NITRO-${VERSION}-x86_64.AppImage bin/
  cd scripts || exit
else
  cd .. || exit
fi
echo "Build complete."
