//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_PROGRAM_H
#define MEGUMIN_PROGRAM_H

#include <vector>
#include <iostream>
#include <set>

#include "Instruction.h"
#include "MachineState.h"
#include "symbol/MachineStateS.h"
#include "RegSet.h"

namespace arm {
    class Program {
    public:
        struct ProgramPosition {
            int basic_block_id;
            int index;
        };
    private:
        /// instructions[i][j]: basic block i, instruction j
        /// instructions[0] is the entry basic block
        std::vector<std::vector<Instruction>> instructions;

        /// [i][j]: basic block i, out going edge to element j
        std::vector<std::vector<int>> out_connections;
        std::vector<std::vector<int>> in_connections;


        /// def_ins[bb_id][inst_index]
        /// def_ins[bb_id][0] stands for entry def ins of the basic block
        /// def_ins[0][0] stands for entry def-ins of the program
        std::vector<std::vector<RegSet>> def_ins;
        /// conceptually equivalent to def_ins[0][0]
        RegSet entry_def_ins;

        // it's a constant during search
//        int instruction_size = 0;

        /// when this function is called, we assume def_ins[basic_block_id][0] is properly set
        void calc_def_ins_for_basic_block(int basic_block_id);

    public:
        Program() = default;
        explicit Program(int basic_block_size);
//        explicit Program(int size);

        void execute(MachineState& state) const;
        void execute(MachineStateS& state) const;

        /// get the instruction count of the program
        [[nodiscard]] int get_size() const;
        [[nodiscard]] int get_basic_block_size() const;
        [[nodiscard]] size_t get_instruction_size(int basic_block_id) const;
        [[nodiscard]] int get_non_nop_size() const;
        int calculate_size() const;
        void add_instruction(int basic_block_id, const Instruction& instruction);
        void set_instruction(int basic_block_id, int index, const Instruction& instruction);
        void set_instruction(ProgramPosition position, const Instruction& instruction);
        void set_instruction_nop(int basic_block_id, int index);
        void swap_instructions(int basic_block_id1, int i1, int basic_block_id2, int i2);
        void swap_instructions(ProgramPosition p1, ProgramPosition p2);

        /// set def_ins[0][0], which is the def-ins at the beginning of the program
        void set_entry_def_ins(const RegSet& def_ins);

        [[nodiscard]] const Instruction& get_instruction_const(int basic_block_id, int index) const;
        [[nodiscard]] const Instruction& get_instruction_const(ProgramPosition position) const;
        void print(std::ostream& os = std::cout) const;

        /// populate def_ins
        /// we assume there is no loop
        void calculate_def_ins();
        [[nodiscard]] const RegSet& get_def_in(int basic_block_id, int index) const;
        [[nodiscard]] const RegSet& get_def_in(ProgramPosition position) const;

        /// In order to ensure the program will not encounter undefined register
        /// the program have to have a set of defined register at the beginning of the program
        /// this function returns the minimum register set which satisfies the above condition
        [[nodiscard]] RegSet get_minimum_def_ins() const;

        [[nodiscard]] bool is_all_integral_instructions() const;
        [[nodiscard]] bool is_all_fp_instructions() const;
    };

    std::ostream& operator<<(std::ostream& os, const arm::Program& prog);
}


#endif //MEGUMIN_PROGRAM_H
