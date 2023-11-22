add_executable(megumin_tests test.cpp)
target_link_libraries(megumin_tests PRIVATE Catch2::Catch2WithMain megumin arm_asm megumin_utils)