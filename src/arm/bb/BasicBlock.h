//
// Created by wormtql on 2023/6/13.
//

#ifndef MEGUMIN_BASICBLOCK_H
#define MEGUMIN_BASICBLOCK_H

#include "Instruction.h"
#include <vector>
#include <iostream>
#include "Program.h"

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

        [[nodiscard]] const std::vector<Instruction>& get_instructions() const {
            return instructions;
        }

        inline void add_instruction(const Instruction& instruction) {
            instructions.push_back(instruction);
        }

        [[nodiscard]] inline std::vector<Instruction>::size_type size() const {
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

        [[nodiscard]] inline int get_start() const { return start; }
        [[nodiscard]] inline int get_end() const { return end; }

        [[nodiscard]] Program to_program() const;
    };
}

std::ostream& operator<<(std::ostream& os, const arm::BasicBlock& bb);


#endif //MEGUMIN_BASICBLOCK_H
