//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_MUTATION_H
#define MEGUMIN_MUTATION_H

#include <random>
#include <Instruction.h>

namespace megumin {
    class Mutation {
    protected:
//        static std::uniform_int_distribution<> dist;
        static arm::Instruction mutate_rn(int value, const arm::Instruction& instruction);
        static arm::Instruction mutate_rd(int value, const arm::Instruction& instruction);
    public:
        Mutation() = default;
        virtual ~Mutation() = default;

        virtual arm::Instruction mutate(const arm::Instruction& instruction) = 0;
    };
}



#endif //MEGUMIN_MUTATION_H
