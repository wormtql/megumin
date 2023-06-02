//
// Created by 58413 on 2023/4/18.
//

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "Program.h"
#include "visitor/InstructionPrinter.h"

namespace arm {
    void Program::execute(MachineState &state) const {
        for (int i = 0; i < size; i++) {
            const Instruction& inst = instructions[i];
            inst.execute(state);
            // todo control flow
        }
    }

    Program::Program(const Program &other) {
        size = other.size;
        memcpy(instructions, other.instructions, sizeof(instructions));
    }

    Program::Program(int size): size(size) {}

    void Program::add_instruction(const Instruction &instruction) {
        assert(size < MAX_INSTRUCTIONS);
        instructions[size] = instruction;
        size++;
    }

    void Program::set_instruction(int index, const Instruction &instruction) {
        assert(index < size);
        instructions[index] = instruction;
    }

    void Program::set_instruction_nop(int index) {
        assert(index < size);
        instructions[index].set_as_nop();
    }

    void Program::swap_instructions(int i1, int i2) {
        assert(i1 < size);
        assert(i2 < size);
        std::swap(instructions[i1], instructions[i2]);
    }

    const Instruction& Program::get_instruction_const(int index) const {
        assert(index < size);
        return instructions[index];
    }

    void Program::print() const {
        InstructionPrinter printer{std::cout};

        for (int i = 0; i < size; i++) {
            printer.visit_instruction(instructions[i]);
            if (i != size - 1)
                std::cout << "\n";
        }
    }
}
