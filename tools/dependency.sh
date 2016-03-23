#!/usr/bin/env bash
git clone -q --depth=1 --branch v3.2 https://github.com/assimp/assimp assimp
cd assimp
cmake .
make -s -j2 
make -s install 
cd ..
