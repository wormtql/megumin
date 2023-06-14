//
// Created by wormtql on 2023/6/13.
//

#ifndef MEGUMIN_BASICBLOCK_H
#define MEGUMIN_BASICBLOCK_H

#include "Instruction.h"
#include <vector>

namespace arm {
    class BasicBlock {
    private:
        std::vector<Instruction> instructions;
        int start = 0;
        int end = 0;
    public:
        BasicBlock() = default;
        explicit BasicBlock(std::vector<Instruction>&& instructions, int start, int end);
        std::vector<Instruction>& get_instructions_mut() {
            return instructions;
        }

        inline void add_instruction(const Instruction& instruction) {
            instructions.push_back(instruction);
        }

        inline std::vector<Instruction>::size_type size() {
            return instructions.size();
        }

        inline void set_start(int s) {
            start = s;
        }

        inline void set_end(int e) {
            end = e;
        }

        inline void clear_instructions() {
            instructions.clear();
        }
    };
}



#endif //MEGUMIN_BASICBLOCK_H
