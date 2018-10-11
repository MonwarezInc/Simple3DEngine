#!/bin/sh

export CC=clang
export CXX=clang++

meson -Db_coverage=true -Dtest=true build && cd build && ninja
