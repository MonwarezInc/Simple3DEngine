#!/bin/sh
export CC=gcc
export CXX=g++

 mkdir build && cd build && cmake .. -GNinja && cmake --build . --target all
