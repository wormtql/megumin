//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_INSTRUCTION_H
#define MEGUMIN_INSTRUCTION_H

#include <cstdint>
#include <optional>
#include "MachineState.h"
#include "Bitvec.h"
#include "RegSet.h"
#include "symbol/MachineStateS.h"

namespace arm {
    enum class InstructionTypeCoarse {
        Integral,
        FPAndSIMD,
    };

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
    public:
        explicit Instruction(bits instruction);
        explicit Instruction(void* data): Instruction(bits{32, *reinterpret_cast<int64_t*>(data)}) {};
        Instruction(const Instruction& other);
        Instruction(): instruction{32, 0} { id = ID++; }
        static Instruction nop() { return Instruction{bits{32, 0}}; }
        [[nodiscard]] InstructionType get_type() const;
        [[nodiscard]] const arm::bits& get_bits() const { return instruction; }
        [[nodiscard]] arm::bits& get_bits_mut() { return instruction; }

        void set_bit(int index, bool value);
        void set_range(int low, int high, int64_t value);
        [[nodiscard]] bool is_set(int index) const;
        void execute(MachineState& state) const;
        void execute(MachineStateS& state) const;
        void set_as_nop();
        void inverse_bit(int index) { instruction.inverse_bit(index); }
        [[nodiscard]] bool is_nop() const;
        [[nodiscard]] bits get_range(int low, int high) const { return instruction.get_range(low, high); }
        [[nodiscard]] bits get_rn() const { return instruction.get_rn(); }
        [[nodiscard]] bits get_rd() const { return instruction.get_rd(); }
        [[nodiscard]] bits get_rm() const { return instruction.get_rm(); }

        [[nodiscard]] std::optional<Reg> get_def_register() const;
        [[nodiscard]] int get_read_registers(Reg result[3]) const;
    };
}



#endif //MEGUMIN_INSTRUCTION_H
