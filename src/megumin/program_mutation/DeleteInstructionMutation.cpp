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
        int basic_block_size = program.get_basic_block_size();
        int basic_block = uniform_int(generator) % basic_block_size;
        int instruction_size = program.get_instruction_size(basic_block);

        if (instruction_size == 0) {
            return MutationResult::failed_result();
        }

        int index;
        if (program.get_instruction_const(basic_block, instruction_size - 1).is_branch_instruction()) {
            if (instruction_size == 1) {
                return MutationResult::failed_result();
            }
            index = uniform_int(generator) % (instruction_size - 1);
        } else {
            index = uniform_int(generator) % instruction_size;
        }

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = {.basic_block_id=basic_block, .index=index};
        result.mutation_instructions[0] = program.get_instruction_const(basic_block, index);

        program.set_instruction_nop(basic_block, index);
        program.calculate_def_ins();

        return result;
    }

    void DeleteInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        if (!result.success) {
            return;
        }

        int basic_block_id = result.mutation_index[0].basic_block_id;
        int index = result.mutation_index[0].index;
        megumin_assert(basic_block_id < program.get_basic_block_size());
        megumin_assert(index < program.get_instruction_size(basic_block_id));

        program.set_instruction(basic_block_id, index, result.mutation_instructions[0]);
        program.calculate_def_ins();
    }
}
