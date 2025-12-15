#!/bin/sh

set -a
. ./.env.build
set +a

cmake -S . \
    -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX
    
cmake --build build