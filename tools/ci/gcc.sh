#!/bin/sh
export CC=gcc
export CXX=g++

 mkdir build && cd build && cmake .. -GNinja -Dasan=OFF && cmake --build . --target all
