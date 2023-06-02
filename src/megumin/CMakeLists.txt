add_library(megumin STATIC "")

file(GLOB megumin_headers "**/*.h" "*.h")
file(GLOB megumin_sources "**/*.cpp" "*.cpp")

target_sources(megumin
        PRIVATE
        ${megumin_sources}
        PUBLIC
        ${megumin_headers}
        )

target_link_libraries(megumin
        arm_asm
        )

target_include_directories(megumin
        PUBLIC
        .
        )

#target_link_libraries(megumin keystone)