//
// Created by 58413 on 2023/6/4.
//

#ifndef MEGUMIN_INCLASSINSTRUCTIONMUTATION_H
#define MEGUMIN_INCLASSINSTRUCTIONMUTATION_H

#include <random>
#include "ProgramMutation.h"

namespace megumin {
    class InClassInstructionMutation: public ProgramMutation {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit InClassInstructionMutation(std::mt19937& generator);

        MutationResult mutate(arm::Program &program) override;

        void undo(arm::Program &program, const MutationResult &result) override;
    };
}



#endif //MEGUMIN_INCLASSINSTRUCTIONMUTATION_H
