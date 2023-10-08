add_library(megumin_utils STATIC "")

target_sources(megumin_utils
    PUBLIC
    megumin_utils.h
    PRIVATE
    megumin_utils.cpp
)

if (WIN32)
    target_sources(megumin_utils
        PRIVATE
        megumin_utils_windows.cpp
        )
else()
    target_sources(megumin_utils
        PRIVATE
        megumin_utils_linux.cpp)
    target_link_libraries(megumin_utils
        keystone
    )    
endif()

target_link_libraries(megumin_utils arm_asm)
target_include_directories(megumin_utils
    PUBLIC
    .
    )