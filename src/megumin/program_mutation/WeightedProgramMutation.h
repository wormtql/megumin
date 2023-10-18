//
// Created by 58413 on 2023/6/5.
//

#ifndef MEGUMIN_WEIGHTEDPROGRAMMUTATION_H
#define MEGUMIN_WEIGHTEDPROGRAMMUTATION_H

#include <vector>
#include <memory>
#include <random>
#include "ProgramMutation.h"
#include "DeleteInstructionMutation.h"
#include "InClassInstructionMutation.h"
#include "RandomInstructionMutation.h"
#include "SwapInstructionMutation.h"

namespace megumin {
    class WeightedProgramMutation: public ProgramMutation {
    public:
        struct Prob {
            int w_delete_instruction = 1;
            int w_in_class_mutation = 1;
            int w_random_instruction = 1;
            int w_swap_instruction = 1;
        };
    private:
        DeleteInstructionMutation delete_instruction_mutation;
        InClassInstructionMutation in_class_instruction_mutation;
        RandomInstructionMutation random_instruction_mutation;
        SwapInstructionMutation swap_instruction_mutation;
        std::vector<ProgramMutation*> mutations;

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        WeightedProgramMutation(
                std::mt19937& generator,
                Prob prob,
                RandomInstructionMutation::RandomInstructionWeight random_weight,
                bool use_fp_instructions,
                bool use_int_instructions
                );
        explicit WeightedProgramMutation(std::mt19937& generator);
        MutationResult mutate(arm::Program &program) override;

        void undo(arm::Program &program, const MutationResult &result) override;

        void set_use_fp_instructions(bool value);
        void set_use_integral_instructions(bool value);
    };
}



#endif //MEGUMIN_WEIGHTEDPROGRAMMUTATION_H
