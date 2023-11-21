add_library(arm_asm STATIC "")

file(GLOB arm_asm_headers "**/*.h" "*.h")
file(GLOB arm_asm_sources "**/*.cpp" "*.cpp")
#message(STATUS ${arm_asm_headers})

target_sources(arm_asm
    PUBLIC
    ${arm_asm_headers}
    PRIVATE
    ${arm_asm_sources}
    )
target_include_directories(arm_asm
    PUBLIC
    .
    include
    visitor)


target_link_libraries(arm_asm
    megumin_utils
    absl::int128
)

if (WIN32)
    target_link_libraries(arm_asm
        ${PROJECT_SOURCE_DIR}/z3_install/bin/libz3.lib
    )
endif ()
