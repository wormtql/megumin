add_library(arm_asm STATIC "")

target_sources(arm_asm
        PUBLIC
        include/MachineState.h
        include/RegBank.h
        include/Instruction.h
        include/Bitvec.h
        include/ArmUtils.h
        dispatch/InstructionDispatch.h
        include/Program.h
        dispatch/InstructionPrint.h
        include/MyFloat.h
        PRIVATE
        MyFloat.cpp
        dispatch/InstructionPrint.cpp
        Program.cpp
        dispatch/InstructionDispatch.cpp
        MachineState.cpp
        RegBank.cpp
        Instruction.cpp
        Bitvec.cpp
        ArmUtils.cpp
        )
target_include_directories(arm_asm
        PUBLIC
        include
        dispatch)
