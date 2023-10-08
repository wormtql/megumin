//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_PROGRAM_H
#define MEGUMIN_PROGRAM_H

#include <vector>
#include <iostream>

#include "Instruction.h"
#include "MachineState.h"
#include "symbol/MachineStateS.h"
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
        void execute(MachineStateS& state) const;

        [[nodiscard]] int get_size() const {
            return instructions.size();
        }
        void add_instruction(const Instruction& instruction);
        void set_instruction(int index, const Instruction& instruction);
        void set_instruction_nop(int index);
        void swap_instructions(int i1, int i2);
        void set_entry_def_ins(const RegSet& def_ins);

        [[nodiscard]] const Instruction& get_instruction_const(int index) const;
        void print(std::ostream& os = std::cout) const;

        void calculate_def_ins();
        [[nodiscard]] const RegSet& get_def_in(int index) const;

        [[nodiscard]] RegSet get_minimum_def_ins() const;
    };

    std::ostream& operator<<(std::ostream& os, const arm::Program& prog);
}


#endif //MEGUMIN_PROGRAM_H
