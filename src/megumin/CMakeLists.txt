add_library(megumin STATIC "")

target_sources(megumin
        PRIVATE
        mutation/Mutation.cpp
        mutation/MutateDataProcessingImm.cpp
        PUBLIC
        mutation/Mutation.h
        mutation/MutateDataProcessingImm.h
        )

target_link_libraries(megumin
        arm_asm
        )