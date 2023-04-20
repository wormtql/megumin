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

    arm::Instruction RandomDataProcessingImmLogical::random_instruction() {
        bits result{32, 0};

        result.set_range(23, 29, 0b100100);

        // op
        int opc = uniform_int_distribution(generator) % 4;
        result.set_range(29, 31, opc);
        // sf
        int sf = uniform_int_distribution(generator) % 2;
        result.set_bit(31, sf);
        // N
        int N = 0;
        if (sf == 0) {
            // then N must be 0
            // this line can be ignored, because result is inited to be all 0
            result.set_bit(22, N);
        } else {
            N = uniform_int_distribution(generator) % 2;
            result.set_bit(22, N);
        }

        // immr
        result.set_range(16, 22, uniform_int_distribution(generator) % (1 << 6));
        // imms
        int level = uniform_int_distribution(generator) % ((sf && N ? 6 : 5)) + 1;
        result.set_range(10, 16, (0b11110ll << level) & (0b111111ll));
        result.set_range(10, 10 + level, uniform_int_distribution(generator) % ((1 << level) - 1));
        // rn
        result.set_range(5, 10, uniform_int_distribution(generator) % (1 << 5));
        // rd
        result.set_range(0, 5, uniform_int_distribution(generator) % (1 << 5));

        return arm::Instruction{result};
    }

    arm::Instruction RandomMoveWideImm::random_instruction() {
        bits result{32, 0};
        result.set_range(23, 29, 0b100101);
        // sf
        int sf = uniform_int(generator) % 2;
        result.set_bit(31, sf);
        // opc
        int opc = uniform_int(generator) % 2;
        if (opc == 0b01) {
            opc = 0b11;
        }
        result.set_range(29, 31, opc);
        // hw
        int hw;
        if (sf) {
            hw = uniform_int(generator) % 4;
        } else {
            hw = uniform_int(generator) % 2;
        }
        result.set_range(21, 23, hw);
        // imm16
        result.set_range(5, 21, uniform_int(generator) % (1 << 16));
        // rd
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return arm::Instruction{result};
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
