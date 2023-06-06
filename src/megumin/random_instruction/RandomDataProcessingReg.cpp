//
// Created by 58413 on 2023/4/22.
//

#include "RandomDataProcessingReg.h"

arm::Instruction megumin::RandomDataProcessing2Source::random_instruction(const arm::Program& program, int index) {
    arm::bits inst{32, 0};
    inst.set_range(21, 29, 0b11010110);

    int temp = uniform_int(generator) % 6;
    if (temp < 2) {
        // udiv or sdiv
        inst.set_range(10, 16, 0b10 + uniform_int(generator) % 2);
    } else {
        inst.set_range(10, 16, 0b1000 + uniform_int(generator) % 2);
    }
    // sf
    inst.set_bit(31, uniform_int(generator) % 2);
    // rm
    const auto& def_ins = program.get_def_in(index);
    inst.set_range(16, 21, def_ins.random_gp(generator));
    // rn
    inst.set_range(5, 10, def_ins.random_gp(generator));
    // rd
    inst.set_range(0, 5, uniform_int(generator) % (1 << 5));

    return arm::Instruction{inst};
}

arm::Instruction megumin::RandomDataProcessing1Source::random_instruction(const arm::Program& program, int index) {
    arm::bits inst{32, 0};
    inst.set_range(21, 31, 0b1011010110);
    inst.set_range(16, 21, 0);

    // sf
    int sf = uniform_int(generator) % 2;
    inst.set_bit(31, sf);
    // opcode
    int i = uniform_int(generator) % 6;
    if (i < 4) {
        if (sf == 0) {
            inst.set_range(10, 16, uniform_int(generator) % 3);
        } else {
            inst.set_range(10, 16, i);
        }
    } else {
        inst.set_range(10, 16, 0b100 + i - 4);
    }
    // rn
    const auto& def_ins = program.get_def_in(index);
    inst.set_range(5, 10, def_ins.random_gp(generator));
    // rd
    inst.set_range(0, 5, uniform_int(generator) % (1 << 5));

    return arm::Instruction{inst};
}

arm::Instruction megumin::RandomDataProcessingRegLogical::random_instruction(const arm::Program &program, int index) {
    arm::bits inst{32, 0};
    inst.set_range(24, 29, 0b01010);

    auto f = [&] () { return uniform_int(generator); };

    bool sf = false;
#ifdef MEGUMIN_INST_64_ONLY
    sf = true;
    inst.set_bit(31, true); // always generate 64-bit instructions
#endif

#ifndef MEGUMIN_INST_64_ONLY
    // sf
    sf = uniform_int(generator) % 2;
    inst.set_bit(31, sf);
#endif
    // opc
    inst.set_range(29, 31, uniform_int(generator) % 4);
    // shift
    inst.set_range(22, 24, f() % 4);
    // N
    inst.set_bit(21, f() % 2);
    // rm
    inst.set_range(16, 21, f() % (1 << 5));
    // imm6
    if (sf) {
        inst.set_range(10, 16, f() % (1 << 6));
    } else {
        inst.set_range(10, 16, f() % (1 << 5));
    }
    // rn
    inst.set_range(5, 10, f() % (1 << 5));
    // rd
    inst.set_range(0, 5, f() % (1 << 5));

    return arm::Instruction{inst};
}
