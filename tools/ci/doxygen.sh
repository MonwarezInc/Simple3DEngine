#!/bin/sh

apt-get update -qq
apt-get install -qq -y doxygen

doxygen Doxyfile
