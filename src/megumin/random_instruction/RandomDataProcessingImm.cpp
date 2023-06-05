//
// Created by 58413 on 2023/4/18.
//
#include <iostream>

#include "Bitvec.h"
#include "RandomDataProcessingImm.h"

using arm::bits;
using namespace std;

namespace megumin {
    arm::Instruction RandomDataProcessingImm::random_instruction(const arm::Program& program, int index) {
        int cat = cat_prob(generator);
        const auto& dispatch = dispatches[cat];
        return dispatch->random_instruction(program, index);
    }

    arm::Instruction RandomDataProcessingImmLogical::random_instruction(const arm::Program& program, int index) {
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
        const auto& def_ins = program.get_def_in(index);
        result.set_range(5, 10, def_ins.random_gp(generator));
        // rd
        result.set_range(0, 5, uniform_int_distribution(generator) % (1 << 5));
        if (result.data0 < 0) {
            printf("123");
        }

        return arm::Instruction{result};
    }

    arm::Instruction RandomMoveWideImm::random_instruction(const arm::Program& program, int index) {
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

    arm::Instruction RandomAddSubImm::random_instruction(const arm::Program& program, int index) {
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
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(5, 10, def_ins.random_gp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int_dist(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomBitfield::random_instruction(const arm::Program& program, int index) {
        bits instruction{32, 0};
        instruction.set_range(23, 29, 0b100110);

        // opc
        instruction.set_range(29, 31, uniform_int(generator) % 3);
        // sf and N
        int sf = uniform_int(generator) % 2;
        instruction.set_bit(31, sf);
        instruction.set_bit(22, sf);

        int x = sf ? (1 << 6) : (1 << 5);

        // immr
        instruction.set_range(16, 22, uniform_int(generator) % x);
        // imms
        instruction.set_range(10, 16, uniform_int(generator) % x);
        // rn
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(5, 10, def_ins.random_gp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomExtract::random_instruction(const arm::Program& program, int index) {
        bits instruction{32, 0};
        instruction.set_range(23, 31, 0b00100111);

        // sf and N
        int sf = uniform_int(generator) % 2;
        instruction.set_bit(31, sf);
        instruction.set_bit(22, sf);

        // rm
        instruction.set_range(16, 21, uniform_int(generator) % (1 << 5));
        // rn
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(5, 10, def_ins.random_gp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));
        // imms
        int x = sf ? (1 << 6) : (1 << 5);
        instruction.set_range(10, 16, uniform_int(generator) % x);
        return arm::Instruction{instruction};
    }
}
