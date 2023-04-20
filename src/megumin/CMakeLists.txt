add_library(megumin STATIC "")

target_sources(megumin
        PRIVATE
        mutation/Mutation.cpp
        mutation/MutateDataProcessingImm.cpp
        search/Search.cpp
        cost/CostFunction.cpp
        cost/LengthCost.cpp
        cost/CorrectnessCost.cpp
        mutation/ProgramMutation.cpp
        mutation/SimpleProgramMutation.cpp
        random_instruction/RandomInstruction.cpp
        random_instruction/RandomDataProcessingImm.cpp
        cost/SimpleCost.cpp
        mutation/SimpleInClassMutation.cpp
        PUBLIC
        mutation/SimpleInClassMutation.h
        cost/SimpleCost.h
        random_instruction/RandomDataProcessingImm.h
        random_instruction/RandomInstruction.h
        mutation/SimpleProgramMutation.h
        mutation/ProgramMutation.h
        cost/CorrectnessCost.h
        cost/LengthCost.h
        cost/CostFunction.h
        search/Search.h
        mutation/Mutation.h
        mutation/MutateDataProcessingImm.h
        )

target_link_libraries(megumin
        arm_asm
        )

target_include_directories(megumin
        PUBLIC
        .
        )