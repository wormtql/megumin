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
        uint64_t id;
        static uint64_t ID;

        void execute_data_processing_imm(MachineState& state) const;
        void execute_data_processing_imm_add_sub_imm(MachineState& state) const;
        void execute_data_processing_imm_add_sub_imm_with_tags(MachineState& state) const;
        void execute_data_processing_imm_logical_imm(MachineState& state) const;
        void execute_data_processing_imm_move_wide_imm(MachineState& state) const;
        void execute_data_processing_imm_bitfield(MachineState& state) const;
        void execute_data_processing_imm_extract(MachineState& state) const;

        void execute_data_processing_reg(MachineState& state) const;
        void execute_data_processing_reg_2_source(MachineState& state) const;
        void execute_data_processing_reg_1_source(MachineState& state) const;

        // floating point and SIMD
        void execute_floating_point_and_simd(MachineState& state) const;

        void execute_floating_point_data_processing(MachineState& state) const;
        void execute_floating_point_data_processing_2(MachineState& state) const;

    public:
        explicit Instruction(bits instruction);
        explicit Instruction(void* data): Instruction(bits{32, *reinterpret_cast<int64_t*>(data)}) {};
        Instruction(const Instruction& other);
        Instruction(): instruction{32, 0} { id = ID++; }
        static Instruction nop() { return Instruction{bits{32, 0}}; }
        [[nodiscard]] InstructionType get_type() const;
        [[nodiscard]] const arm::bits& get_bits() const { return instruction; }
        InstructionType2 get_type2() const;

        void set_bit(int index, bool value);
        void set_range(int low, int high, int64_t value);
        [[nodiscard]] bool get_bit(int index) const;
        void execute(MachineState& state) const;
        void set_as_nop();
        void inverse_bit(int index) { instruction.inverse_bit(index); }
        [[nodiscard]] bool is_nop() const;
        [[nodiscard]] bits get_range(int low, int high) const { return instruction.get_range(low, high); }
    };
}



#endif //MEGUMIN_INSTRUCTION_H
