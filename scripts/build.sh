#!/bin/sh

cmake -S . \
    -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=C:/Apps/Portables/aerell
    
cmake --build build