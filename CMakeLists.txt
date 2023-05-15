cmake_minimum_required(VERSION 3.20)
project(megumin)

set(CMAKE_CXX_STANDARD 20)
#SET(CMAKE_BUILD_TYPE "Release")

#find_package(LLVM REQUIRED CONFIG PATHS F:\\llvm-install)
#
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
#include_directories(${LLVM_INCLUDE_DIRS})
#separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
#add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(keystone/include)
link_directories(keystone)

add_subdirectory(src/arm)
add_subdirectory(src/playground)
add_subdirectory(src/megumin)

# keystone

add_subdirectory(keystone)


