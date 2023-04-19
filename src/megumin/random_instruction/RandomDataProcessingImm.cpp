//
// Created by 58413 on 2023/4/18.
//

#include "Bitvec.h"
#include "RandomDataProcessingImm.h"

using arm::bits;

namespace megumin {
    arm::Instruction RandomDataProcessingImm::random_instruction() {
        int cat = cat_prob(generator);
        auto dispatch = dispatches[cat];
        return dispatch->random_instruction();
    }

    RandomAddSubImm::RandomAddSubImm(std::mt19937 &generator): generator(generator), uniform_int_dist() {}

    arm::Instruction RandomAddSubImm::random_instruction() {
        int add_or_sub = uniform_int_dist(generator) % 2;
        bits instruction{32, 0};

        instruction.set_range(23, 29, 0b100010);

        // op
        instruction.set_bit(30, add_or_sub);
        // sf
        instruction.set_bit(31, uniform_int_dist(generator) % 2);
        // S
        instruction.set_bit(29, uniform_int_dist(generator) % 2);
        // sh
        instruction.set_bit(22, uniform_int_dist(generator) % 2);

        // imm12
        instruction.set_range(10, 22, uniform_int_dist(generator) % (1 << 12));
        // rn
        instruction.set_range(5, 10, uniform_int_dist(generator) % (1 << 5));
        // rd
        instruction.set_range(0, 5, uniform_int_dist(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }
}
