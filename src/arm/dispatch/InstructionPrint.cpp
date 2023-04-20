//
// Created by 58413 on 2023/4/19.
//

#include "Bitvec.h"
#include "InstructionPrint.h"
#include "ArmUtils.h"

using arm::bits;

void arm::InstructionPrint::dispatch_data_processing_imm_add_sub(const arm::Instruction &instruction) {
    bool sf = instruction.get_bit(31);
    bool op = instruction.get_bit(30);
    bool S = instruction.get_bit(29);
    bool sh = instruction.get_bit(22);
    bits imm12 = instruction.get_range(10, 22);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    if (!op) {
        os << "add";
    } else {
        os << "sub";
    }
    if (S) {
        os << "s";
    }

    os << " ";

    if (sf) os << "x"; else os << "w";
    os << rd.as_u64();
    os << ", ";
    if (sf) os << "x"; else os << "w";
    os << rn.as_u64();
    os << ", ";

    os << "#";
    os << imm12.as_u64();
    if (sh) {
        os << ", lsl #12";
    }
}

void arm::InstructionPrint::dispatch_nop(const arm::Instruction &instruction) {
    os << "nop";
}

void arm::InstructionPrint::dispatch_data_processing_imm_logical(const arm::Instruction& instruction) {
    bool sf = instruction.get_bit(31);
    bits opc = instruction.get_range(29, 31);
    bool N = instruction.get_bit(22);
    bits immr = instruction.get_range(16, 22);
    bits imms = instruction.get_range(10, 16);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    int n = rn.as_u64();
    int d = rd.as_u64();

    int datasize = sf ? 64 : 32;

    auto imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;
    if (opc == 0b00) {
        os << "and";
    } else if (opc == 0b01) {
        os << "orr";
    } else if (opc == 0b10) {
        os << "eor";
    } else if (opc == 0b11) {
        os << "ands";
    }
    os << " ";

    auto reg_name = sf ? "x" : "w";
    os << reg_name << d << ", ";
    os << reg_name << n << ", ";
    os << "#" << imm.as_u64();
}

void arm::InstructionPrint::dispatch_data_processing_move_wide(const arm::Instruction &instruction) {
    bits opc = instruction.get_range(29, 31);
    bool sf = instruction.get_bit(31);
    bits hw = instruction.get_range(21, 23);
    bits imm16 = instruction.get_range(5, 21);
    bits rd = instruction.get_range(0, 5);

    if (opc == 0b00) {
        os << "movn";
    } else if (opc == 0b10) {
        os << "movz";
    } else if (opc == 0b11) {
        os << "movk";
    } else {
        assert(false);
    }

    auto reg = sf ? "x" : "w";
    os << reg << rd.as_u64() << ", ";
    os << "#" << imm16.as_u64();
    if (hw != 0) {
        unsigned int shift = hw.as_u32() * 16;
        os << ", LSL #" << shift;
    }
}
