//
// Created by 58413 on 2023/6/5.
//

#include "WeightedProgramMutation.h"

namespace megumin {
    WeightedProgramMutation::WeightedProgramMutation(std::mt19937 &generator)
        : WeightedProgramMutation(generator, {}, {}, true, true)
    {}

    void WeightedProgramMutation::set_use_integral_instructions(bool value) {
        random_instruction_mutation.set_use_integral_instructions(value);
    }

    void WeightedProgramMutation::set_use_fp_instructions(bool value) {
        random_instruction_mutation.set_use_fp_instructions(value);
    }

    WeightedProgramMutation::WeightedProgramMutation(
            std::mt19937 &generator,
            Prob prob,
            RandomInstructionMutation::RandomInstructionWeight random_weight,
            bool use_fp_instructions,
            bool use_int_instructions
            )
        : generator(generator),
          delete_instruction_mutation(generator),
          in_class_instruction_mutation(generator),
          random_instruction_mutation(generator, random_weight),
          swap_instruction_mutation(generator)
    {
        random_instruction_mutation.set_use_fp_instructions(use_fp_instructions);
        random_instruction_mutation.set_use_integral_instructions(use_int_instructions);
        for (int i = 0; i < prob.w_delete_instruction; i++) {
            mutations.push_back(&delete_instruction_mutation);
        }
        for (int i = 0; i < prob.w_in_class_mutation; i++) {
            mutations.push_back(&in_class_instruction_mutation);
        }
        for (int i = 0; i < prob.w_random_instruction; i++) {
            mutations.push_back(&random_instruction_mutation);
        }
        for (int i = 0; i < prob.w_swap_instruction; i++) {
            mutations.push_back(&swap_instruction_mutation);
        }
    }
}

megumin::MutationResult megumin::WeightedProgramMutation::mutate(arm::Program &program) {
    int index = uniform_int(generator) % mutations.size();

    auto result = mutations[index]->mutate(program);
    result.tag = index;
    return result;
}

void megumin::WeightedProgramMutation::undo(arm::Program &program, const megumin::MutationResult &result) {
    mutations[result.tag]->undo(program, result);
}
