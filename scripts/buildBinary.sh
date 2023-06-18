#!/bin/bash

# Builds an AppImage from the project

cd ..
mkdir -p build
cd build/ || exit
export QT_QPA_PLATFORM_PLUGIN_PATH=/lib/x86_64-linux-gnu/qt6/plugins/platforms/
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=/usr/lib
make -j$(nproc)
mkdir share/doc/libc6
make DESTDIR=../appdir -j$(nproc) install
find ../appdir/
if [[ $? -eq 0 ]]; then
  cd ..
  mkdir -p appdir/usr/share/doc/
  mkdir -p appdir/usr/share/doc/libc6
  touch appdir/usr/share/doc/libc6/copyright
  export PATH=/usr/lib/qt6/bin/:$PATH

  if [[ ! -f linuxdeployqt-continuous-x86_64.AppImage ]]; then
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod u+x linuxdeployqt-continuous-x86_64.AppImage
  fi
  ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/*.desktop -unsupported-allow-new-glibc -appimage
  mv NITRO-be7a949-x86_64.AppImage nitro.AppImage
  cd scripts || exit
else
  cd .. || exit
fi
