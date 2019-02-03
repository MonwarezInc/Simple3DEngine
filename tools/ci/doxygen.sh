#!/bin/sh

apt-get update -qq
apt-get install -qq -y doxygen
apt-get install -qq -y graphviz

meson -Dtest=true -Ddocumentation=true build

cd build
ninja docs
