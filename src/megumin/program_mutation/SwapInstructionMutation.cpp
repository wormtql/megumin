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
        int basic_block_size = program.get_basic_block_size();
        int basic_block1 = uniform_int(generator) % basic_block_size;
        int basic_block2 = uniform_int(generator) % basic_block_size;
        int index1 = uniform_int(generator) % static_cast<int>(program.get_instruction_size(basic_block1));
        int index2 = uniform_int(generator) % static_cast<int>(program.get_instruction_size(basic_block2));
        arm::Program::ProgramPosition position1 = {basic_block1, index1};
        arm::Program::ProgramPosition position2 = {basic_block2, index2};

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = position1;
        result.mutation_index[1] = position2;

        if (index1 == index2) {
            return result;
        }

        program.swap_instructions(position1, position2);
        program.calculate_def_ins();

        return result;
    }

    void SwapInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        megumin_assert(result.mutation_index[0].basic_block_id < program.get_basic_block_size());
        megumin_assert(result.mutation_index[1].basic_block_id < program.get_basic_block_size());
        megumin_assert(result.mutation_index[0].index < program.get_instruction_size(result.mutation_index[0].basic_block_id));
        megumin_assert(result.mutation_index[1].index < program.get_instruction_size(result.mutation_index[1].basic_block_id));

        program.swap_instructions(result.mutation_index[0], result.mutation_index[1]);
        program.calculate_def_ins();
    }
}
