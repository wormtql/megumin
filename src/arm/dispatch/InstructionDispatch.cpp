//
// Created by 58413 on 2023/4/17.
//

#include "InstructionDispatch.h"

namespace arm {
    void InstructionDispatch::dispatch(const Instruction& instruction) {
        if (instruction.is_nop()) {
            if (need_dispatch_nop()) {
                dispatch_nop(instruction);
            }
            return;
        }

        auto type = instruction.get_type();
        if (type == InstructionType::DataProcessingImm) {
            if (need_dispatch_data_processing_imm()) {
                dispatch_data_processing_imm(instruction);
            }
        } else if (type == InstructionType::DataProcessingReg) {
            if (need_dispatch_data_processing_reg()) {
                dispatch_data_processing_reg(instruction);
            }
        } else if (type == InstructionType::DataProcessingSIMD) {
            if (need_dispatch_simd()) {
                dispatch_simd(instruction);
            }
        }
        // todo
    }

    void InstructionDispatch::dispatch_simd(const Instruction& instruction) {
        bits op0 = instruction.get_range(28, 32);
        bits op1 = instruction.get_range(23, 25);
        bits op2 = instruction.get_range(19, 23);
        bits op3 = instruction.get_range(10, 19);

        bool floating_point_flag1 = op0[0] && !op0[2] && !op1[1] && op2[2];

        if (floating_point_flag1 && op3[{0, 5}] == 0b10000) {
            if (need_dispatch_fp_data_processing1()) {
                dispatch_fp_data_processing1(instruction);
            }
        } else {
            // todo
            assert(false);
        }
    }

    void InstructionDispatch::dispatch_data_processing_imm(const Instruction &instruction) {
        bits op0 = instruction.get_range(23, 26);
        if (op0 == 0b010) {
            if (need_dispatch_data_processing_imm_add_sub()) {
                dispatch_data_processing_imm_add_sub(instruction);
            }
        } else if (op0 == 0b011) {
            if (need_dispatch_data_processing_imm_add_sub_with_tags()) {
                dispatch_data_processing_imm_add_sub_with_tags(instruction);
            }
        } else if (op0 == 0b100) {
            if (need_dispatch_data_processing_imm_logical()) {
                dispatch_data_processing_imm_logical(instruction);
            }
        } else if (op0 == 0b101) {
            if (need_dispatch_data_processing_move_wide()) {
                dispatch_data_processing_move_wide(instruction);
            }
        } else if (op0 == 0b110) {
            if (need_dispatch_data_processing_bitfield()) {
                dispatch_data_processing_bitfield(instruction);
            }
        } else if (op0 == 0b111) {
            if (need_dispatch_data_processing_extract()) {
                dispatch_data_processing_extract(instruction);
            }
        }
    }

    void InstructionDispatch::dispatch_data_processing_reg(const Instruction& instruction) {
        bool op0 = instruction.get_bit(30);
        bool op1 = instruction.get_bit(28);
        bits op2 = instruction.get_range(21, 25);
        bits op3 = instruction.get_range(10, 16);

        if (op0 == 0 && op1 == 1 && op2 == 0b0110) {
            if (need_dispatch_data_processing_2source()) {
                dispatch_data_processing_2source(instruction);
            }
        } else if (op0 == 1 && op1 == 1 && op2 == 0b0110) {
            if (need_dispatch_data_processing_1source()) {
                dispatch_data_processing_1source(instruction);
            }
        }
        // todo
    }
}