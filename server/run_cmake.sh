#!/bin/bash

FLAGS=""

rm -rf build 
mkdir build
if [ $(uname -m) == "x86_64" ] ; then
        FLAGS="-A x64"
fi
cd build &&
cmake $FLAGS ..
