#!/bin/sh

apt-get update -qq
apt-get install -qq -y doxygen
apt-get install -qq -y graphviz

doxygen Doxyfile
