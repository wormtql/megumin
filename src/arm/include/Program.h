//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_PROGRAM_H
#define MEGUMIN_PROGRAM_H

#include <vector>

#include "Instruction.h"
#include "MachineState.h"
#include "RegSet.h"

namespace arm {
    class Program {
    private:
        std::vector<Instruction> instructions;

        std::vector<RegSet> def_ins;
        RegSet entry_def_ins;
    public:
        Program() = default;
        explicit Program(int size);
        Program(int size, const RegSet& def_ins0);

        void execute(MachineState& state) const;
        [[nodiscard]] int get_size() const {
            return instructions.size();
        }
        void add_instruction(const Instruction& instruction);
        void set_instruction(int index, const Instruction& instruction);
        void set_instruction_nop(int index);
        void swap_instructions(int i1, int i2);

        [[nodiscard]] const Instruction& get_instruction_const(int index) const;
        void print() const;

        void calculate_def_ins();
        const RegSet& get_def_in(int index);
    };
}



#endif //MEGUMIN_PROGRAM_H
