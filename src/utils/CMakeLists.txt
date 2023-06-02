add_library(megumin_utils STATIC "")

file(GLOB megumin_utils_headers "**/*.h" "*.h")
file(GLOB megumin_utils_sources "**/*.cpp" "*.cpp")

message(STATUS ${megumin_utils_sources})

target_sources(megumin_utils
        PUBLIC
        ${megumin_utils_headers}
        PRIVATE
        ${megumin_utils_sources}
)
target_include_directories(megumin_utils
        PUBLIC
        .
        )