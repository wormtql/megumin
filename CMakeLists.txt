cmake_minimum_required(VERSION 3.16)
project(megumin)

set(CMAKE_CXX_STANDARD 20)
#SET(CMAKE_BUILD_TYPE "Release")

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (WIN32)
    add_definitions(-DMEGUMIN_IS_WINDOWS)
    include_directories(keystone_install/include)

    # setup z3
    include_directories(z3_install/include)
    link_directories(z3_install/bin)
    file(COPY ${PROJECT_SOURCE_DIR}/z3_install/bin/libz3.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
else()
    find_package(LLVM REQUIRED CONFIG PATHS /mnt/disk2/yaoxilai/llvm/16.0.4/llvm_install NO_DEFAULT_PATH)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
    include_directories(${LLVM_INCLUDE_DIRS})
    separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
    add_definitions(${LLVM_DEFINITIONS_LIST})

    message(STATUS "using static keystone")
    # keystone
    include_directories(keystone_install/include)
    link_directories(keystone_install/lib)
endif()

add_subdirectory(src/arm)
add_subdirectory(src/playground)
add_subdirectory(src/megumin)
add_subdirectory(src/utils)

# keystone

# add_subdirectory(keystone)


