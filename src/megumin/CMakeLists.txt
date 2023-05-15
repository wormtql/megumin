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
        random_instruction/RandomDataProcessingReg.cpp
        cost/SimpleCost.cpp
        mutation/SimpleInClassMutation.cpp
        random_instruction/RandomInstructionTop.cpp
        mutation/MutateDataProcessingReg.cpp
        random_instruction/RandomFPAndSIMD.cpp
        mutation/MutateFPDataProcessing.cpp
        utils/utils.cpp
        PUBLIC
        utils/utils.h
        mutation/MutateFPDataProcessing.h
        random_instruction/RandomFPAndSIMD.h
        mutation/MutateDataProcessingReg.h
        random_instruction/RandomInstructionTop.h
        random_instruction/RandomDataProcessingReg.h
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

#target_include_directories(megumin PUBLIC ${PROJECT_SOURCE_DIR}/keystone/include)
#target_link_libraries(megumin ${PROJECT_SOURCE_DIR}/keystone/keystone.lib)
#target_include_directories(megumin PUBLIC E:\\codes\\keystone\\install\\include)
#target_link_libraries(megumin E:\\codes\\keystone\\install\\lib\\keystone.lib)
#target_link_libraries(megumin ${PROJECT_SOURCE_DIR}/keystone/keystone.dll)
target_link_libraries(megumin keystone)