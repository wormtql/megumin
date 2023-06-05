//
// Created by 58413 on 2023/4/22.
//

#include "RandomDataProcessingReg.h"

namespace megumin {

}

megumin::RandomDataProcessingReg::RandomDataProcessingReg(std::mt19937 &generator, Prob prob)
    : generator(generator),
      discrete{{
          prob.w_2_source,
          prob.w_1_source,
      }}
{
    dispatches.push_back(std::make_unique<RandomDataProcessing2Source>(generator));
    dispatches.push_back(std::make_unique<RandomDataProcessing1Source>(generator));
}

arm::Instruction megumin::RandomDataProcessingReg::random_instruction(const arm::Program& program, int index) {
    int index2 = discrete(generator);
    auto& dispatch = dispatches[index2];
    return dispatch->random_instruction(program, index);
}

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
