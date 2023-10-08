
function(add_megumin_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
            arm_asm
            megumin
            )
    if (WIN32)
        target_link_libraries(${name} ${PROJECT_SOURCE_DIR}/z3_install/bin/libz3.lib)
    endif()

    if (NOT WIN32)
        target_link_libraries(${name} keystone)
    endif()
endfunction()

add_megumin_executable(auto_opt)
add_megumin_executable(main)
add_megumin_executable(z3_test)
add_megumin_executable(z3_2)


# add_executable(playground main.cpp)
# add_executable(main2 main2.cpp)
# add_executable(main3 main3.cpp)
# add_executable(main4 main4.cpp)


# target_link_libraries(playground
#         arm_asm
#         )

# target_link_libraries(main2
#         arm_asm
#         megumin
#         )

# target_link_libraries(main3
#         arm_asm
#         megumin
#         )

# target_link_libraries(main4
#         arm_asm
#         megumin
#         )



# include_directories(${LLVM_INCLUDE_DIRS})
# message(STATUS "Using LLVM include dirs ${LLVM_INCLUDE_DIRS}")

# if (LLVM_FOUND)
#     llvm_map_components_to_libnames(main3_libs support core irreader target targetparser mc AllTargetsCodeGens AllTargetsAsmParsers AllTargetsInfos AllTargetsDescs AllTargetsMCAs)
#     MESSAGE(STATUS "main3 lib: ${main3_libs}")
#     target_link_libraries(main3 ${main3_libs})
# endif()
#target_include_directories(main3 ${PROJECT_SOURCE_DIR}/3rd/llvm/llvm/include)
#target_include_directories(main3 PRIVATE ${LLVM_INCLUDE_DIRS})



# if (NOT WIN32)
#     target_link_libraries(main2 keystone)
#     target_link_libraries(main4 keystone)
# endif()

if (WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/keystone_install/keystone.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
