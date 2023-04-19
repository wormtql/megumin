//
// Created by 58413 on 2023/4/19.
//

#include "Bitvec.h"
#include "InstructionPrint.h"

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
