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
        calculate_def_ins();
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

    void Program::print() const {
        InstructionPrinter printer{std::cout};

        for (int i = 0; i < instructions.size(); i++) {
            printer.visit_instruction(instructions[i]);
            if (i != instructions.size() - 1)
                std::cout << "\n";
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

    const RegSet& Program::get_def_in(int index) {
        return def_ins[index];
    }
}
