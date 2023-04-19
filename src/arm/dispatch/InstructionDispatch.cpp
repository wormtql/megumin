//
// Created by 58413 on 2023/4/17.
//

#include "InstructionDispatch.h"

namespace arm {
    void InstructionDispatch::dispatch(const Instruction& instruction) {
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
                dispatch_data_processing_imm_logical();
            }
        } else if (op0 == 0b101) {

        } else if (op0 == 0b110) {

        } else if (op0 == 0b111) {

        }
    }

    void InstructionDispatch::dispatch_data_processing_reg(const Instruction& instruction) {
        // todo
    }
}