//
// Created by 58413 on 2023/6/4.
//

#include "InClassInstructionMutation.h"
#include "instruction_mutation/SimpleInstructionMutation.h"
#include "megumin_utils.h"

namespace megumin {
    InClassInstructionMutation::InClassInstructionMutation(std::mt19937 &generator)
        : generator(generator),
          simple_instruction_mutation(generator)
    {}

    MutationResult InClassInstructionMutation::mutate(arm::Program &program) {
        int basic_block_size = program.get_basic_block_size();
        int basic_block = uniform_int(generator) % basic_block_size;
        int instruction_size = program.get_instruction_size(basic_block);
        int index = uniform_int(generator) % instruction_size;

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = {.basic_block_id=basic_block, .index=index};
        result.mutation_instructions[0] = program.get_instruction_const(basic_block, index);

        simple_instruction_mutation.result = arm::Instruction::nop();
        simple_instruction_mutation.program = &program;
        simple_instruction_mutation.position.basic_block_id = basic_block;
        simple_instruction_mutation.position.index = index;
        simple_instruction_mutation.visit_instruction(result.mutation_instructions[0]);
        program.set_instruction(basic_block, index, simple_instruction_mutation.result);

        program.calculate_def_ins();

        return result;
    }

    void InClassInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        int basic_block_id = result.mutation_index[0].basic_block_id;
        int index = result.mutation_index[0].index;
        megumin_assert(basic_block_id < program.get_basic_block_size());
        megumin_assert(index < program.get_instruction_size(basic_block_id));

        program.set_instruction(basic_block_id, index, result.mutation_instructions[0]);

        program.calculate_def_ins();
    }
}
