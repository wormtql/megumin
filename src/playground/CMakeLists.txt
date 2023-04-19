add_executable(playground main.cpp)
add_executable(main2 main2.cpp)

target_link_libraries(playground
        arm_asm
        )

target_link_libraries(main2
        arm_asm
        megumin
        )

