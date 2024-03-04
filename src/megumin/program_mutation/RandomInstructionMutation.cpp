//
// Created by 58413 on 2023/6/3.
//

#include "RandomInstructionMutation.h"
#include <random_instruction/RandomDataProcessingImm.h>
#include <random_instruction/RandomDataProcessingReg.h>
#include <random_instruction/RandomFPAndSIMD.h>

namespace megumin {
    RandomInstructionMutation::RandomInstructionMutation(std::mt19937 &generator, RandomInstructionWeight weight)
        :generator(generator)
    {
        // data processing imm random instructions
        for (int i = 0; i < weight.dp_imm_add_sub; i++) {
            random_functions.push_back(std::make_unique<RandomAddSubImm>(generator));
        }
        for (int i = 0; i < weight.dp_imm_add_sub_with_tags; i++) {
            // todo
        }
        for (int i = 0; i < weight.dp_imm_logical; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessingImmLogical>(generator));
        }
        for (int i = 0; i < weight.dp_imm_move_wide; i++) {
            random_functions.push_back(std::make_unique<RandomMoveWideImm>(generator));
        }
        for (int i = 0; i < weight.dp_imm_bitfield; i++) {
            random_functions.push_back(std::make_unique<RandomBitfield>(generator));
        }
        for (int i = 0; i < weight.dp_imm_extract; i++) {
            random_functions.push_back(std::make_unique<RandomExtract>(generator));
        }

        // data processing reg random instructions
        for (int i = 0; i < weight.dp_reg_1source; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessing1Source>(generator));
        }
        for (int i = 0; i < weight.dp_reg_2source; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessing2Source>(generator));
        }
        for (int i = 0; i < weight.dp_reg_logical_shifted_reg; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessingRegLogical>(generator));
        }
        for (int i = 0; i < weight.dp_reg_add_sub_shifted_reg; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessingRegAddSubShiftedReg>());
        }
        for (int i = 0; i < weight.dp_reg_add_sub_with_carry; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessingRegAddSubWithCarry>());
        }
        for (int i = 0; i < weight.dp_reg_cond_select; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessingRegCondSelect>());
        }
        for (int i = 0; i < weight.dp_reg_3source; i++) {
            random_functions.push_back(std::make_unique<RandomDataProcessing3Source>());
        }
        integral_instruction_bound = random_functions.size();

        // fp and simd
        for (int i = 0; i < weight.fp_and_simd_dp_1source; i++) {
            random_functions.push_back(std::make_unique<RandomFPDataProcessing1>(generator));
        }
        for (int i = 0; i < weight.fp_and_simd_dp_2source; i++) {
            random_functions.push_back(std::make_unique<RandomFPDataProcessing2>(generator));
        }
        for (int i = 0; i < weight.fp_and_simd_dp_3source; i++) {
            random_functions.push_back(std::make_unique<RandomFPDataProcessing3>(generator));
        }
        for (int i = 0; i < weight.fp_and_simd_imm; i++) {
            random_functions.push_back(std::make_unique<RandomFPImm>(generator));
        }
        for (int i = 0; i < weight.fp_compare; i++) {
            random_functions.push_back(std::make_unique<RandomFPCompare>(generator));
        }
    }
}

megumin::MutationResult megumin::RandomInstructionMutation::mutate(arm::Program &program) {
    int basic_block_size = program.get_basic_block_size();
    int basic_block = uniform_int(generator) % basic_block_size;
    int instruction_size = program.get_instruction_size(basic_block);
    int index = uniform_int(generator) % instruction_size;

    int random_index = 0;
    int random_function_size = (int) random_functions.size();
    if (use_integral_instructions && use_fp_instructions) {
        random_index = uniform_int(generator) % random_function_size;
    } else if (use_integral_instructions) {
        random_index = uniform_int(generator) % integral_instruction_bound;
    } else if (use_fp_instructions) {
        random_index = uniform_int(generator) % (random_function_size - integral_instruction_bound) + integral_instruction_bound;
    } else {
        // this should not happen
        assert(false);
        random_index = uniform_int(generator) % random_function_size;
    }

    arm::Program::ProgramPosition position = { .basic_block_id=basic_block, .index=index };
    arm::Instruction instruction = random_functions[random_index]->random_instruction(program, position);

    MutationResult result;
    result.success = true;
    result.mutation_index[0] = position;
    result.mutation_instructions[0] = program.get_instruction_const(position);

    program.set_instruction(position, instruction);
    program.calculate_def_ins();

    return result;
}

void megumin::RandomInstructionMutation::undo(arm::Program &program, const megumin::MutationResult &result) {
    int basic_block_id = result.mutation_index[0].basic_block_id;
    int index = result.mutation_index[0].index;
    megumin_assert(basic_block_id < program.get_basic_block_size());
    megumin_assert(index < program.get_instruction_size(basic_block_id));

    program.set_instruction(result.mutation_index[0], result.mutation_instructions[0]);
    program.calculate_def_ins();
}

void megumin::RandomInstructionMutation::set_use_integral_instructions(bool value) {
    this->use_integral_instructions = value;
}

void megumin::RandomInstructionMutation::set_use_fp_instructions(bool value) {
    this->use_fp_instructions = value;
}
