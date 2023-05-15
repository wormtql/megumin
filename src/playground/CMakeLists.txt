add_executable(playground main.cpp)
add_executable(main2 main2.cpp)

target_link_libraries(playground
        arm_asm
        )

target_link_libraries(main2
        arm_asm
        megumin
        )

#add_executable(main3 main3.cpp)
#include_directories(${LLVM_INCLUDE_DIRS})
#message(STATUS "Using LLVM include dirs ${LLVM_INCLUDE_DIRS}")

#llvm_map_components_to_libnames(main3_libs support core irreader target targetparser mc AllTargetsCodeGens AllTargetsAsmParsers AllTargetsInfos AllTargetsDescs AllTargetsMCAs)
#MESSAGE(STATUS "main3 lib: ${main3_libs}")
#target_link_libraries(main3 ${main3_libs})
#target_include_directories(main3 ${PROJECT_SOURCE_DIR}/3rd/llvm/llvm/include)
#target_include_directories(main3 PRIVATE ${LLVM_INCLUDE_DIRS})

add_executable(main4 main4.cpp)
target_link_libraries(main4 keystone)