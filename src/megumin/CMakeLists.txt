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
        ${Z3_LIBRARIES}
        )

target_include_directories(megumin
        PUBLIC
        .
        )

#add_definitions(-DMEGUMIN_INST_64_ONLY)

#target_link_libraries(megumin keystone)