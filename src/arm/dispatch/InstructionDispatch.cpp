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
        }
        // todo
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
        // todo
    }
}