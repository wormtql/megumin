//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_INSTRUCTION_H
#define MEGUMIN_INSTRUCTION_H

#include <cstdint>
#include "MachineState.h"
#include "Bitvec.h"

namespace arm {
    enum class InstructionType {
        Reserved,
        Unallocated,
        SVE,
        DataProcessingImm,
        BranchExceptionSystem,
        LoadAndStore,
        DataProcessingReg,
        DataProcessingSIMD,
    };

    enum class InstructionType2 {
        DataProcessingImmAddSub,
        DataProcessingImmAddSubWithTags,
        DataProcessingImmLogical,
        DataProcessingImmMoveWide,
        DataProcessingImmBitfield,
        DataProcessingImmExtract,
    };

    class Instruction {
    private:
        arm::bits instruction;

        void execute_data_processing_imm(MachineState& state);
        void execute_data_processing_imm_add_sub_imm(MachineState& state);
        void execute_data_processing_imm_add_sub_imm_with_tags(MachineState& state);
        void execute_data_processing_imm_logical_imm(MachineState& state);
//        void execute_data_processing_imm_move_wide_imm(MachineState& state);

        void execute_data_processing_reg(MachineState& state);
    public:
        explicit Instruction(bits instruction);
        InstructionType get_type();
        InstructionType2 get_type2();

        void set_bit(int index, bool value);
        void set_range(int low, int high, int64_t value);
        bool get_bit(int index) const;
        void execute(MachineState& state);
    };
}



#endif //MEGUMIN_INSTRUCTION_H
