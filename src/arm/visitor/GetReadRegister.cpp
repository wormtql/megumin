//
// Created by 58413 on 2023/6/6.
//

#include "GetReadRegister.h"
#include "megumin_utils.h"

void arm::GetReadRegister::add_rn(const arm::Instruction& instruction) {
    bits rn = instruction.get_range(5, 10);
    results[size] = Reg::gp(rn.as_i32());
    size++;
}

void arm::GetReadRegister::add_rm(const Instruction &instruction) {
    bits rm = instruction.get_range(16, 21);
    results[size] = Reg::gp(rm.as_i32());
    size++;
}

void arm::GetReadRegister::add_rn_fp(const Instruction &instruction) {
    bits rn = instruction.get_range(5, 10);
    results[size] = Reg::fp(rn.as_i32());
    size++;
}

void arm::GetReadRegister::add_rm_fp(const Instruction &instruction) {
    bits rm = instruction.get_range(16, 21);
    results[size] = Reg::fp(rm.as_i32());
    size++;
}

void arm::GetReadRegister::visit_nop(const arm::Instruction &instruction) {
    size = 0;
}

void arm::GetReadRegister::visit_dp_imm_add_sub(const arm::Instruction &instruction) {
    add_rn(instruction);
}

void arm::GetReadRegister::visit_dp_imm_add_sub_with_tags(const arm::Instruction &instruction) {
    megumin::megumin_todo();
}

void arm::GetReadRegister::visit_dp_imm_logical(const arm::Instruction &instruction) {
    add_rn(instruction);
}

void arm::GetReadRegister::visit_dp_imm_move_wide(const arm::Instruction &instruction) {
    size = 0;
}

void arm::GetReadRegister::visit_dp_imm_bitfield(const arm::Instruction &instruction) {
    add_rn(instruction);
}

void arm::GetReadRegister::visit_dp_imm_extract(const arm::Instruction &instruction) {
    add_rn(instruction);
}

void arm::GetReadRegister::visit_dp_reg_2source(const arm::Instruction &instruction) {
    add_rn(instruction);
    add_rm(instruction);
}

void arm::GetReadRegister::visit_dp_reg_1source(const arm::Instruction &instruction) {
    add_rn(instruction);
}

void arm::GetReadRegister::visit_fp_simd_dp_1source(const arm::Instruction &instruction) {
    add_rn_fp(instruction);
}

void arm::GetReadRegister::visit_fp_simd_dp_2source(const arm::Instruction &instruction) {
    add_rn_fp(instruction);
    add_rm_fp(instruction);
}

void arm::GetReadRegister::visit_dp_reg_logical_shifted_reg(const arm::Instruction &instruction) {
    add_rn(instruction);
    add_rm(instruction);
}

void arm::GetReadRegister::visit_dp_reg_add_sub_shifted_reg(const arm::Instruction &instruction) {
    add_rn(instruction);
    add_rm(instruction);
}

void arm::GetReadRegister::visit_dp_reg_add_sub_with_carry(const arm::Instruction &instruction) {
    add_rn(instruction);
    add_rm(instruction);
}

void arm::GetReadRegister::visit_dp_reg_cond_select(const arm::Instruction &instruction) {
    add_rn(instruction);
    add_rm(instruction);
}

void arm::GetReadRegister::visit_dp_reg_3source(const arm::Instruction &instruction) {
    bits op31 = instruction.get_range(21, 24);
    bool o0 = instruction.is_set(15);
    bits ra = instruction.get_range(10, 15);
    bits op = op31.concat(bits::from_bools({o0}));

    if (op == 0b0100 || op == 0b1100) {
        add_rn(instruction);
        add_rm(instruction);
    } else {
        add_rn(instruction);
        add_rm(instruction);
        results[size] = Reg::gp(ra.as_i32());
        size++;
    }
}
