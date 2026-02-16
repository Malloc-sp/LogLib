#!/bin/bash

sudo bash uninstall.sh
echo "初始化环境成功"
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
sudo cmake --install . --config Release