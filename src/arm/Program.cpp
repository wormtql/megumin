//
// Created by 58413 on 2023/4/18.
//

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "megumin_utils.h"
#include "Program.h"
#include "RegSet.h"
#include "visitor/InstructionPrinter.h"

using megumin::megumin_assert;

namespace arm {
    std::ostream& operator<<(std::ostream& os, const arm::Program& prog) {
        prog.print(os);

        return os;
    }
}

namespace arm {
    void Program::execute(MachineState &state) const {
        for (const auto& instruction: instructions) {
            instruction.execute(state);
            // todo control flow
        }
    }

    Program::Program(int size) {
        for (int i = 0; i < size; i++) {
            instructions.push_back(Instruction::nop());
        }
    }

    Program::Program(int size, const RegSet& def_ins0) {
        for (int i = 0; i < size; i++) {
            instructions.push_back(Instruction::nop());
        }
        entry_def_ins = def_ins0;
//        calculate_def_ins();
    }

    void Program::add_instruction(const Instruction &instruction) {
        instructions.push_back(instruction);
    }

    void Program::set_instruction(int index, const Instruction &instruction) {
        megumin_assert(index < instructions.size());
        instructions[index] = instruction;
    }

    void Program::set_instruction_nop(int index) {
        megumin_assert(index < instructions.size());
        instructions[index].set_as_nop();
    }

    void Program::swap_instructions(int i1, int i2) {
        megumin_assert(i1 < instructions.size());
        megumin_assert(i2 < instructions.size());
        std::swap(instructions[i1], instructions[i2]);
    }

    const Instruction& Program::get_instruction_const(int index) const {
        megumin_assert(index < instructions.size());
        return instructions[index];
    }

    void Program::print(std::ostream& os) const {
        InstructionPrinter printer{os};

        for (int i = 0; i < instructions.size(); i++) {
            printer.visit_instruction(instructions[i]);
            if (i != instructions.size() - 1)
                os << "\n";
        }
    }

    void Program::calculate_def_ins() {
        def_ins.resize(instructions.size() + 1);
        def_ins[0] = entry_def_ins;
        for (int i = 0; i < instructions.size(); i++) {
            def_ins[i + 1] = def_ins[0];
            const Instruction& instruction = instructions[i];
            auto def_reg = instruction.get_def_register();
            if (def_reg.has_value()) {
                def_ins[i + 1].add_reg(def_reg.value());
            }
        }
    }

    const RegSet& Program::get_def_in(int index) const {
        return def_ins[index];
    }

    void Program::set_entry_def_ins(const RegSet& def_ins0) {
        entry_def_ins = def_ins0;
    }

    RegSet Program::get_minimum_def_ins() const {
        RegSet gen;
        RegSet result;
        for (int i = 0; i < instructions.size(); i++) {
            const Instruction& instruction = instructions[i];

            Reg read_regs[3];
            int read_reg_size = instruction.get_read_registers(read_regs);
            for (int j = 0; j < read_reg_size; j++) {
                if (!gen.have_reg(read_regs[j])) {
                    result.add_reg(read_regs[j]);
                }
            }

            auto def_reg = instruction.get_def_register();
            if (def_reg.has_value()) {
                gen.add_reg(def_reg.value());
            }
        }
        return result;
    }

    void Program::execute(MachineStateS &state) const {
        for (const auto& instruction: instructions) {
            instruction.execute(state);
            // todo control flow
        }
    }

    bool Program::is_all_integral_instructions() const {
        for (const auto& instruction: instructions) {
            auto ty = instruction.get_type();
            if (ty != InstructionType::DataProcessingImm && ty != InstructionType::DataProcessingReg) {
                return false;
            }
        }
        return true;
    }

    bool Program::is_all_fp_instructions() const {
        for (const auto& instruction: instructions) {
            auto ty = instruction.get_type();
            if (ty != InstructionType::DataProcessingSIMD) {
                return false;
            }
        }
        return true;
    }
}
