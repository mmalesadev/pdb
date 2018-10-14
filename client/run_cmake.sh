#!/bin/bash

FLAGS=""

rm -rf build &&
mkdir build &&
cd build &&
if [ $(uname -m) == "x86_64" ] ; then
	FLAGS="-A x64"
fi
if [ "$1" == "DEBUG" ] ; then
	FLAGS="$FLAGS -DBoost_DEBUG=ON"
fi
cmake $FLAGS ..

