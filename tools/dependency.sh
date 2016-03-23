#!/bin/bash
cd ~/
git clone --depth=1 --branch v3.2 https://github.com/assimp/assimp.git assimp
cd assimp
cmake .
make -j2 
sudo make install 
cd ..
