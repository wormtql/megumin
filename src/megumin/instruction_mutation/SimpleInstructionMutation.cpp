//
// Created by 58413 on 2023/6/4.
//

#include "SimpleInstructionMutation.h"
#include "megumin_utils.h"

namespace megumin {
    SimpleInstructionMutation::SimpleInstructionMutation(std::mt19937& generator)
        : mutate_dp_imm_add_sub(generator),
          mutate_dp_imm_logical(generator),
          mutate_dp_imm_move_wide(generator),
          mutate_dp_imm_bitfield(generator),
          mutate_dp_imm_extract(generator),
          mutate_dp_reg_1source(generator),
          mutate_dp_reg_2source(generator),
          mutate_fp_simd_dp_2source(generator),
          mutate_fp_simd_dp_1source(generator)
    {}

    void SimpleInstructionMutation::visit_nop(const arm::Instruction &instruction) {
        result = arm::Instruction::nop();
    }

    void SimpleInstructionMutation::visit_dp_imm_add_sub(const arm::Instruction &instruction) {
        result = mutate_dp_imm_add_sub.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_imm_add_sub_with_tags(const arm::Instruction &instruction) {
        megumin_todo();
    }

    void SimpleInstructionMutation::visit_dp_imm_logical(const arm::Instruction &instruction) {
        result = mutate_dp_imm_logical.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_imm_move_wide(const arm::Instruction &instruction) {
        result = mutate_dp_imm_move_wide.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_imm_bitfield(const arm::Instruction &instruction) {
        result = mutate_dp_imm_bitfield.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_imm_extract(const arm::Instruction &instruction) {
        result = mutate_dp_imm_extract.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_reg_2source(const arm::Instruction &instruction) {
        result = mutate_dp_reg_2source.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_dp_reg_1source(const arm::Instruction &instruction) {
        result = mutate_dp_reg_1source.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_fp_simd_dp_1source(const arm::Instruction &instruction) {
        result = mutate_fp_simd_dp_1source.mutate(instruction);
    }

    void SimpleInstructionMutation::visit_fp_simd_dp_2source(const arm::Instruction &instruction) {
        result = mutate_fp_simd_dp_2source.mutate(instruction);
    }
}
