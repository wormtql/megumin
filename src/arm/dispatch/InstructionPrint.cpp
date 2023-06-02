//
// Created by 58413 on 2023/4/19.
//

#include <iostream>

#include "Bitvec.h"
#include "InstructionPrint.h"
#include "ArmUtils.h"

using arm::bits;
using namespace std;

void arm::InstructionPrint::dispatch_data_processing_imm_add_sub(const arm::Instruction &instruction) {
    bool sf = instruction.is_set(31);
    bool op = instruction.is_set(30);
    bool S = instruction.is_set(29);
    bool sh = instruction.is_set(22);
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
    bool sf = instruction.is_set(31);
    bits opc = instruction.get_range(29, 31);
    bool N = instruction.is_set(22);
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
    bool sf = instruction.is_set(31);
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
    os << " ";

    auto reg = sf ? "x" : "w";
    os << reg << rd.as_u64() << ", ";
    os << "#" << imm16.as_u64();
    if (hw != 0) {
        unsigned int shift = hw.as_u32() * 16;
        os << ", LSL #" << shift;
    }
}

void arm::InstructionPrint::dispatch_data_processing_bitfield(const arm::Instruction &instruction1) {
    const bits& instruction = instruction1.get_bits();
    bool sf = instruction.is_set(31);
    bits opc = instruction.get_range(29, 31);
    bool N = instruction.is_set(22);
    bits immr = instruction.get_range(16, 22);
    bits imms = instruction.get_range(10, 16);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    if (opc == 00) {
        os << "sbfm";
    } else if (opc == 01) {
        os << "bfm";
    } else if (opc == 10) {
        os << "ubfm";
    } else {
        assert(false);
    }
    os << " ";

    auto reg = sf ? "x" : "w";
    os << reg << rd.as_u64() << ", ";
    os << reg << rn.as_u64() << ", ";
    os << "#" << immr.as_u64() << ", ";
    os << "#" << imms.as_u64();
}

void arm::InstructionPrint::dispatch_data_processing_extract(const arm::Instruction &instruction) {
    const bool sf = instruction.is_set(31);
    const bits rm = instruction.get_range(16, 21);
    const bits imms = instruction.get_range(10, 16);
    const bits rn = instruction.get_range(5, 10);
    const bits rd = instruction.get_range(0, 5);

    const auto reg = sf ? "x" : "w";

    os << "extr ";
    os << reg << rd.as_u64() << ", ";
    os << reg << rn.as_u64() << ", ";
    os << reg << rm.as_u64() << ", ";
    os << "#" << imms.as_u64();
}

void arm::InstructionPrint::dispatch_data_processing_2source(const arm::Instruction &instruction) {
    bool sf = instruction.is_set(31);
    bits rm = instruction.get_range(16, 21);
    bits opcode = instruction.get_range(10, 16);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    if (opcode == 0b000010) {
        os << "udiv";
    } else if (opcode == 0b000011) {
        os << "sdiv";
    } else if (opcode == 0b001000) {
        os << "lslv";
    } else if (opcode == 0b001001) {
        os << "lsrv";
    } else if (opcode == 0b001010) {
        os << "asrv";
    } else if (opcode == 0b001011) {
        os << "rorv";
    }
    os << " ";
    auto reg = sf ? "x" : "w";
    os << reg << rd.as_u64() << ", ";
    os << reg << rn.as_u64() << ", ";
    os << reg << rm.as_u64();
}

void arm::InstructionPrint::dispatch_data_processing_1source(const arm::Instruction &instruction) {
    bool sf = instruction.is_set(31);
    bits opcode2 = instruction.get_range(16, 21);
    bits opcode = instruction.get_range(10, 16);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    if (opcode2 == 0 && opcode == 0) {
        os << "rbit";
    } else if (opcode2 == 0 && opcode == 0b000001) {
        os << "rev16";
    } else if (opcode2 == 0 && opcode == 0b000010) {
        if (sf) {
            os << "rev32";
        } else {
            os << "rev";
        }
    } else if (opcode2 == 0 && opcode == 0b000011) {
        if (sf) {
            os << "rev";
        } else {
            assert(false);
        }
    } else if (opcode2 == 0 && opcode == 0b000100) {
        os << "clz";
    } else if (opcode2 == 0 && opcode == 0b000101) {
        os << "cls";
    }
    os << " ";

    auto reg = sf ? "x" : "w";
    os << reg << rd.as_u64() << ", ";
    os << reg << rn.as_u64();
}

void arm::InstructionPrint::dispatch_fp_data_processing1(const arm::Instruction &instruction) {
    bool M = instruction.is_set(31);
    bool S = instruction.is_set(29);
    bits ptype = instruction.get_range(22, 24);
    bits opcode = instruction.get_range(15, 21);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    assert(!M);
    assert(!S);

    if (opcode == 0b000000) {
        os << "fmov";
    } else if (opcode == 0b000001) {
        os << "fabs";
    } else if (opcode == 0b000010) {
        os << "fneg";
    } else if (opcode == 0b000011) {
        os << "fsqrt";
    } else if (opcode == 0b000101) {
        os << "fcvt";
    } else if (opcode == 0b000111) {
        os << "fcvt";
    }
    // todo

    char reg = '0';
    if (ptype == 0b00) {
        reg = 'S';
    } else if (ptype == 0b01) {
        reg = 'D';
    } else if (ptype == 0b11) {
        reg = 'H';
    } else {
        assert(false);
    }

    os << " ";
    os << reg << rd.as_u64() << ", ";
    os << reg << rn.as_u64();
}

void arm::InstructionPrint::dispatch_fp_data_processing2(const Instruction& instruction) {
    bool M = instruction.is_set(31);
    bool S = instruction.is_set(29);
    bits ptype = instruction.get_range(22, 24);
    bits rm = instruction.get_range(16, 21);
    bits opcode = instruction.get_range(12, 16);
    bits rn = instruction.get_range(5, 10);
    bits rd = instruction.get_range(0, 5);

    if (opcode == 0b0000) {
        os << "fmul";
    } else if (opcode == 0b0001) {
        os << "fdiv";
    } else if (opcode == 0b0010) {
        os << "fadd";
    } else if (opcode == 0b0011) {
        os << "fsub";
    } else if (opcode == 0b0100) {
        os << "fmax";
    } else if (opcode == 0b0101) {
        os << "fmin";
    } else if (opcode == 0b0110) {
        os << "fmaxnm";
    } else if (opcode == 0b0111) {
        os << "fminnm";
    } else if (opcode == 0b1000) {
        os << "fnmul";
    }

    char reg = '0';
    if (ptype == 0b00) {
        reg = 'S';
    } else if (ptype == 0b01) {
        reg = 'D';
    } else if (ptype == 0b11) {
        reg = 'H';
    } else {
        assert(false);
    }

    os << " ";
    os << reg << rd.as_u32() << ", ";
    os << reg << rn.as_u32() << ", ";
    os << reg << rm.as_u32();
}
