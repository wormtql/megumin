//
// Created by 58413 on 2023/6/3.
//

#include "SwapInstructionMutation.h"
#include "megumin_utils.h"

namespace megumin {
    SwapInstructionMutation::SwapInstructionMutation(std::mt19937 &generator)
        :generator(generator)
    {}

    MutationResult SwapInstructionMutation::mutate(arm::Program &program) {
        int instruction_size = program.get_size();
        int index1 = uniform_int(generator) % instruction_size;
        int index2 = uniform_int(generator) % instruction_size;

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = index1;
        result.mutation_index[1] = index2;

        if (index1 == index2) {
            return result;
        }

        program.swap_instructions(index1, index2);
        return result;
    }

    void SwapInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        megumin_assert(result.mutation_index[0] < program.get_size());
        megumin_assert(result.mutation_index[1] < program.get_size());
        program.swap_instructions(result.mutation_index[0], result.mutation_index[1]);
    }
}
