//
// Created by 58413 on 2023/6/16.
//

#include "CanHandleThisInst.h"

void arm::CanHandleThisInst::visit_nop(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_imm_add_sub(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_imm_add_sub_with_tags(const arm::Instruction &instruction) {
    result = false;
}

void arm::CanHandleThisInst::visit_dp_imm_logical(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_imm_move_wide(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_imm_bitfield(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_imm_extract(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_2source(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_1source(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_logical_shifted_reg(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_add_sub_shifted_reg(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_add_sub_with_carry(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_dp_reg_cond_select(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_fp_simd_dp_1source(const arm::Instruction &instruction) {
    result = false;
}

void arm::CanHandleThisInst::visit_fp_simd_dp_2source(const arm::Instruction &instruction) {
    result = false;
}

void arm::CanHandleThisInst::visit_dp_reg_3source(const arm::Instruction &instruction) {
    result = true;
}

void arm::CanHandleThisInst::visit_fp_simd_imm(const arm::Instruction &instruction) {
    result = false;
}

void arm::CanHandleThisInst::visit_fp_simd_dp_3source(const arm::Instruction &instruction) {
    result = false;
}

void arm::CanHandleThisInst::visit_fp_compare(const arm::Instruction &instruction) {
    result = false;
}
