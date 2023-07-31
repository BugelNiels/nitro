#!/bin/bash
# Cleans up build files

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"

cd ..
rm build -rf
cd appdir || exit
rm AppRun
rm *.desktop
cd usr || exit
rm bin/ -rf
rm lib/ -rf
rm share/doc -rf
rm plugins/ -rf
rm translations/ -rf
cd "$initial_loc"
