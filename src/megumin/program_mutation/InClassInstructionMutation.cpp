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
        int size = program.get_size();
        int index = uniform_int(generator) % size;

        MutationResult result;
        result.success = true;
        result.mutation_index[0] = index;
        result.mutation_instructions[0] = program.get_instruction_const(index);

        simple_instruction_mutation.result = arm::Instruction::nop();
        simple_instruction_mutation.program = &program;
        simple_instruction_mutation.index = index;
        simple_instruction_mutation.visit_instruction(result.mutation_instructions[0]);
        program.set_instruction(index, simple_instruction_mutation.result);

        program.calculate_def_ins();

        return result;
    }

    void InClassInstructionMutation::undo(arm::Program &program, const MutationResult &result) {
        megumin_assert(result.mutation_index[0] < program.get_size());

        program.set_instruction(result.mutation_index[0], result.mutation_instructions[0]);

        program.calculate_def_ins();
    }
}
