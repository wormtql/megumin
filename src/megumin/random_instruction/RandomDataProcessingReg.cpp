//
// Created by 58413 on 2023/4/22.
//

#include "RandomDataProcessingReg.h"

namespace megumin {

}

megumin::RandomDataProcessingReg::RandomDataProcessingReg(std::mt19937 &generator, Prob prob)
    : generator(generator),
      discrete{{
          prob.w_2_source
      }}
{
    dispatches.push_back(std::make_unique<RandomDataProcessing2Source>(generator));
}

arm::Instruction megumin::RandomDataProcessingReg::random_instruction() {
    int index = discrete(generator);
    auto& dispatch = dispatches[index];
    return dispatch->random_instruction();
}

arm::Instruction megumin::RandomDataProcessing2Source::random_instruction() {
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
    inst.set_range(16, 21, uniform_int(generator) % (1 << 5));
    // rn
    inst.set_range(5, 10, uniform_int(generator) % (1 << 5));
    // rd
    inst.set_range(0, 5, uniform_int(generator) % (1 << 5));

    return arm::Instruction{inst};
}