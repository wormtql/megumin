mkdir keystone_build
mkdir keystone_install

current=`pwd`

cd keystone_build
cmake ../3rd/keystone
cmake --build .
cmake -DCMAKE_INSTALL_PREFIX=${pwd}/keystone_install -P cmake_install.cmake
