//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_INSTRUCTIONMUTATION_H
#define MEGUMIN_INSTRUCTIONMUTATION_H

#include <random>
#include <Instruction.h>
#include <Program.h>

namespace megumin {
    class InstructionMutation {
    protected:
//        static std::uniform_int_distribution<> dist;
        static arm::Instruction mutate_rn(int value, const arm::Instruction& instruction);
        static arm::Instruction mutate_rd(int value, const arm::Instruction& instruction);
    public:
        InstructionMutation() = default;
        virtual ~InstructionMutation() = default;

//        virtual arm::Instruction mutate(const arm::Instruction& instruction) = 0;
        virtual arm::Instruction mutate(const arm::Program& program, int index) = 0;
    };
}



#endif //MEGUMIN_INSTRUCTIONMUTATION_H
