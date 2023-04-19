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

        void execute_data_processing_imm(MachineState& state) const;
        void execute_data_processing_imm_add_sub_imm(MachineState& state) const;
        void execute_data_processing_imm_add_sub_imm_with_tags(MachineState& state) const;
        void execute_data_processing_imm_logical_imm(MachineState& state) const;
//        void execute_data_processing_imm_move_wide_imm(MachineState& state);

        void execute_data_processing_reg(MachineState& state) const;
    public:
        explicit Instruction(bits instruction);
        Instruction(): instruction{32, 0} {}
        static Instruction nop() { return Instruction{bits{32, 0}}; }
        [[nodiscard]] InstructionType get_type() const;
        InstructionType2 get_type2() const;

        void set_bit(int index, bool value);
        void set_range(int low, int high, int64_t value);
        [[nodiscard]] bool get_bit(int index) const;
        void execute(MachineState& state) const;
        void set_as_nop();
        [[nodiscard]] bool is_nop() const;
        [[nodiscard]] bits get_range(int low, int high) const { return instruction.get_range(low, high); }
    };
}



#endif //MEGUMIN_INSTRUCTION_H
