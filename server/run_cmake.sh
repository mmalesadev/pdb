#!/bin/bash

FLAGS=""

rm -rf build &&
mkdir build &&
cd build &&
cmake $FLAGS ..