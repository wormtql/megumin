//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_MUTATION_H
#define MEGUMIN_MUTATION_H

#include <Instruction.h>

namespace megumin {
    class Mutation {
    public:
        Mutation() = default;
        virtual ~Mutation() = default;

        virtual arm::Instruction mutate(const arm::Instruction& instruction) = 0;
    };
}



#endif //MEGUMIN_MUTATION_H
