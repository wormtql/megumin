//
// Created by 58413 on 2024/3/6.
//

#ifndef MEGUMIN_RESIZEMUTATION_H
#define MEGUMIN_RESIZEMUTATION_H

#include <random>
#include "ProgramMutation.h"

namespace megumin {
    class ResizeMutation: public ProgramMutation {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit ResizeMutation(std::mt19937& generator);

        MutationResult mutate(arm::Program &program) override;

        void undo(arm::Program &program, const MutationResult &result) override;
    };
}

#endif //MEGUMIN_RESIZEMUTATION_H
