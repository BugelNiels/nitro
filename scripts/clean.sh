#!/bin/bash

# Cleans up build files

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
cd ..
cd ..
cd scripts || exit