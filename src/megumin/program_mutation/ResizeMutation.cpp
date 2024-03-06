//
// Created by 58413 on 2024/3/6.
//

#include "ResizeMutation.h"

namespace megumin {
    ResizeMutation::ResizeMutation(std::mt19937 &generator): generator(generator) {}

    MutationResult ResizeMutation::mutate(arm::Program &program) {
        int basic_block_size = program.get_basic_block_size();
        int basic_block1 = uniform_int(generator) % basic_block_size;
        int basic_block2 = uniform_int(generator) % basic_block_size;
        int instruction_size1 = static_cast<int>(program.get_instruction_size(basic_block1));
        int instruction_size2 = static_cast<int>(program.get_instruction_size(basic_block2));

        auto random_nop_position1 = program.random_nop_position_in_basic_block(basic_block1, uniform_int(generator));
        auto random_nop_position2 = program.random_nop_position_in_basic_block(basic_block2, uniform_int(generator));
        if (random_nop_position1.has_value()) {
            // move the nop to BB2
            int nop_position = random_nop_position1.value();

            MutationResult result;
            result.success = true;
            result.mutation_index[0] = {basic_block1, nop_position};
            result.mutation_index[1] = {basic_block2, instruction_size2};

            program.add_instruction(basic_block2, arm::Instruction::nop());
            program.delete_instruction(basic_block1, nop_position);
            program.calculate_def_ins();

            return result;
        } else if (random_nop_position2.has_value()) {
            // move nop to BB1
            int nop_position = random_nop_position2.value();

            MutationResult result;
            result.success = true;
            result.mutation_index[0] = {basic_block2, nop_position};
            result.mutation_index[1] = {basic_block1, instruction_size1};

            program.add_instruction(basic_block1, arm::Instruction::nop());
            program.delete_instruction(basic_block2, nop_position);
            program.calculate_def_ins();

            return result;
        } else {
            // do nothing
            return MutationResult::failed_result();
        }
    }

    void ResizeMutation::undo(arm::Program &program, const MutationResult &result) {
        if (!result.success) {
            return;
        }

        program.insert_instruction(result.mutation_index[0], arm::Instruction::nop());
        program.delete_instruction(result.mutation_index[1]);
    }
}
