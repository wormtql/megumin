cmake_minimum_required(VERSION 3.20)
project(megumin)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src/arm)
add_subdirectory(src/playground)
add_subdirectory(src/megumin)

# keystone
#add_subdirectory(3rd/keystone)