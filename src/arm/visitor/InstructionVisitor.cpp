#include "InstructionVisitor.h"
#include "Bitvec.h"

namespace arm {
    void InstructionVisitor::visit_instruction(const Instruction& instruction) {
        if (instruction.is_nop()) {
            visit_nop(instruction);
        } else {
            auto type = instruction.get_type();
            if (type == InstructionType::DataProcessingImm) {
                visit_data_processing_imm(instruction);
            } else if (type == InstructionType::DataProcessingReg) {
                visit_data_processing_reg(instruction);
            } else if (type == InstructionType::DataProcessingSIMD) {
                visit_fp_and_simd(instruction);
            }
        }
    }

    void InstructionVisitor::visit_data_processing_imm(const Instruction& instruction) {
        bits op0 = instruction.get_range(23, 26);
        if (op0 == 0b010) {
            visit_dp_imm_add_sub(instruction);
        } else if (op0 == 0b011) {
            visit_dp_imm_add_sub_with_tags(instruction);
        } else if (op0 == 0b100) {
            visit_dp_imm_logical(instruction);
        } else if (op0 == 0b101) {
            visit_dp_imm_move_wide(instruction);
        } else if (op0 == 0b110) {
            visit_dp_imm_bitfield(instruction);
        } else if (op0 == 0b111) {
            visit_dp_imm_extract(instruction);
        }
    }

    void InstructionVisitor::visit_data_processing_reg(const Instruction& instruction) {
        bool op0 = instruction.is_set(30);
        bool op1 = instruction.is_set(28);
        bits op2 = instruction.get_range(21, 25);
        bits op3 = instruction.get_range(10, 16);

        if (op0 == 0 && op1 == 1 && op2 == 0b0110) {
            visit_dp_reg_2source(instruction);
        } else if (op0 == 1 && op1 == 1 && op2 == 0b0110) {
            visit_dp_reg_1source(instruction);
        } else if (op1 == 0 && op2[3] == 0) {
            visit_dp_reg_logical_shifted_reg(instruction);
        } else if (op1 == 0 && op2[3] == 1 && op2[0] == 0) {
            visit_dp_reg_add_sub_shifted_reg(instruction);
        } else if (op1 == 1 && op2 == 0 && op3 == 0) {
            visit_dp_reg_add_sub_with_carry(instruction);
        } else if (op1 == 1 && op2 == 0b0100) {
            visit_dp_reg_cond_select(instruction);
        }
        // todo
    }

    void InstructionVisitor::visit_fp_and_simd(const Instruction& instruction) {
        bits op0 = instruction.get_range(28, 32);
        bits op1 = instruction.get_range(23, 25);
        bits op2 = instruction.get_range(19, 23);
        bits op3 = instruction.get_range(10, 19);

        bool floating_point_flag1 = op0[0] && !op0[2] && !op1[1] && op2[2];

        if (floating_point_flag1 && op3[{0, 5}] == 0b10000) {
            visit_fp_simd_dp_1source(instruction);
        } else if (floating_point_flag1 && op3[{0, 2}] == 0b10) {
            visit_fp_simd_dp_2source(instruction);
        } else if (floating_point_flag1 && op3[{0, 3}] == 0b100) {
            visit_fp_simd_imm(instruction);
        }
        else {
            // todo
//            assert(false);
        }
    }
}