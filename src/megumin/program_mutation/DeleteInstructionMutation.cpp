//
// Created by 58413 on 2023/6/3.
//

#include "DeleteInstructionMutation.h"
#include "megumin_utils.h"

namespace megumin {
    DeleteInstructionMutation::DeleteInstructionMutation(std::mt19937 &generator)
        :generator(generator)
    {}

    MutationResult DeleteInstructionMutation::mutate(arm::Program &program) {
        int size = program.get_size();
        int index = uniform_int(generator) % size;

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = index;
        result.mutation_instructions[0] = program.get_instruction_const(index);

        program.set_instruction_nop(index);
        return result;
    }

    void DeleteInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        megumin_assert(result.mutation_index[0] < program.get_size());

        program.set_instruction(result.mutation_index[0], result.mutation_instructions[0]);
    }
}



