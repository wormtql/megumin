//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_PROGRAM_H
#define MEGUMIN_PROGRAM_H

#include <vector>

#include "Instruction.h"
#include "MachineState.h"

const int MAX_INSTRUCTIONS = 10;

namespace arm {
    class Program {
    private:
        Instruction instructions[MAX_INSTRUCTIONS];
        int size = 0;
    public:
        Program() = default;
        explicit Program(int size);
        Program(const Program& other);

        void execute(MachineState& state) const;
        [[nodiscard]] int get_size() const {
            return size;
        }
        void add_instruction(const Instruction& instruction);
        void set_instruction(int index, const Instruction& instruction);
        void set_instruction_nop(int index);
        void swap_instructions(int i1, int i2);

        const Instruction& get_instruction_const(int index) const;
        void print() const;
    };
}



#endif //MEGUMIN_PROGRAM_H
