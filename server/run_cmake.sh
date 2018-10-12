#!/bin/bash

FLAGS=""

rm -rf build &&
mkdir build &&
cd build &&
if [ $(uname -m) == "x86_64" ] ; then
	FLAGS="-A x64"
fi
cmake $FLAGS ..