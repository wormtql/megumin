//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_DELETEINSTRUCTIONMUTATION_H
#define MEGUMIN_DELETEINSTRUCTIONMUTATION_H

#include <random>
#include "ProgramMutation.h"

namespace megumin {
    class DeleteInstructionMutation: public ProgramMutation {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit DeleteInstructionMutation(std::mt19937& generator);

        MutationResult mutate(arm::Program &program) override;

        void undo(arm::Program &program, const MutationResult &result) override;
    };
}



#endif //MEGUMIN_DELETEINSTRUCTIONMUTATION_H
