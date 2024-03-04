//
// Created by 58413 on 2023/4/14.
//

#include <cmath>

#include "Instruction.h"
#include "Bitvec.h"
#include "ArmUtils.h"
#include "MyFloat.h"
#include "visitor/GetDefRegister.h"
#include "visitor/InstructionExecution.h"
#include "visitor/GetReadRegister.h"
#include "symbol/InstructionExecutionS.h"

namespace arm {
    uint64_t Instruction::ID = 0;

    Instruction::Instruction(bits instruction): instruction(instruction) {
        id = ID++;
    }

    Instruction::Instruction(const Instruction &other) {
        id = ID++;
        instruction = other.instruction;
    }

    InstructionType Instruction::get_type() const {
//        Bitvec op0 = get_range(instruction, 25, 29);
        bits op0 = instruction.get_range(25, 29);
        if (op0 == 0) {
            return InstructionType::Reserved;
        } else if (op0 == 0b0001 || op0 == 0b0011) {
            return InstructionType::Reserved;
        } else if (op0 == 0b0010) {
            return InstructionType::SVE;
        } else if ((op0 >> 1) == 0b100) {
            return InstructionType::DataProcessingImm;
        } else if ((op0 >> 1) == 0b101) {
            return InstructionType::BranchExceptionSystem;
        } else if (op0.is_set(2) && !op0.is_set(0)) {
            return InstructionType::LoadAndStore;
        } else if ((op0 & 0b111) == 0b101) {
            return InstructionType::DataProcessingReg;
        } else if ((op0 & 0b111) == 0b111) {
            return InstructionType::DataProcessingSIMD;
        }
        assert(false);
    }

    void Instruction::execute(MachineState &state) const {
        if (instruction.data0 == 0) {
            // nop
            return;
        }

        InstructionExecution execution{state};
        execution.execute(*this);
        std::optional<int> next_bb = execution.get_next_basic_block();
        if (next_bb.has_value()) {
            state.current_basic_block = next_bb.value();
            // start from first instruction
            state.pc = 0;
        } else {
            state.pc++;
        }
    }

    void Instruction::execute(MachineStateS &state) const {
        if (instruction.data0 == 0) {
            // nop
            return;
        }

        InstructionExecutionS execution{state};
        execution.visit_instruction(*this);
    }

    void Instruction::set_bit(int index, bool value) {
        instruction.set_bit(index, value);
    }

    bool Instruction::is_set(int index) const {
        assert(index < 32);
        return instruction.is_set(index);
    }

    void Instruction::set_range(int low, int high, int64_t value) {
        instruction.set_range(low, high, value);
    }

    void Instruction::set_as_nop() {
        instruction.data0 = 0;
    }

    bool Instruction::is_nop() const {
        return instruction.data0 == 0;
    }

    std::optional<Reg> Instruction::get_def_register() const {
        GetDefRegister get_def_register;
        get_def_register.visit_instruction(*this);
        if (get_def_register.result.reg_type == RegType::None) {
            return {};
        } else {
            return get_def_register.result;
        }
    }

    int Instruction::get_read_registers(Reg *result) const {
        GetReadRegister get_read_register;
        get_read_register.results;
        get_read_register.visit_instruction(*this);
        for (int i = 0; i < get_read_register.size; i++) {
            result[i] = get_read_register.results[i];
        }
        return get_read_register.size;
    }
}
