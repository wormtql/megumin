//
// Created by 58413 on 2023/6/3.
//

#include "GetDefRegister.h"

void arm::GetDefRegister::set_rd(const Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::set_rd_fp(const Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::fp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_imm_add_sub(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_imm_add_sub_with_tags(const arm::Instruction &instruction) {
}

void arm::GetDefRegister::visit_dp_imm_logical(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_imm_move_wide(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_imm_bitfield(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_imm_extract(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_reg_2source(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_reg_1source(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_fp_simd_dp_1source(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::fp(rd.as_i32());
}

void arm::GetDefRegister::visit_fp_simd_dp_2source(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::fp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_reg_logical_shifted_reg(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_reg_add_sub_shifted_reg(const arm::Instruction &instruction) {
    bits rd = instruction.get_range(0, 5);
    result = Reg::gp(rd.as_i32());
}

void arm::GetDefRegister::visit_dp_reg_add_sub_with_carry(const arm::Instruction &instruction) {
    set_rd(instruction);
}

void arm::GetDefRegister::visit_dp_reg_cond_select(const arm::Instruction &instruction) {
    set_rd(instruction);
}

void arm::GetDefRegister::visit_dp_reg_3source(const arm::Instruction &instruction) {
    set_rd(instruction);
}

void arm::GetDefRegister::visit_fp_simd_imm(const arm::Instruction &instruction) {
    set_rd(instruction);
}

void arm::GetDefRegister::visit_fp_simd_dp_3source(const arm::Instruction &instruction) {
    set_rd_fp(instruction);
}

void arm::GetDefRegister::visit_fp_compare(const arm::Instruction &instruction) {
    // no def
}
