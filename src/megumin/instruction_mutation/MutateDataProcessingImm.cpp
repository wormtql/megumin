//
// Created by 58413 on 2023/4/17.
//

#include <cstdlib>
#include <iostream>

#include <ArmUtils.h>
#include <Bitvec.h>
#include "MutateDataProcessingImm.h"

using arm::bits;
using namespace std;

// mutate add/sub imm
namespace megumin {
    arm::Instruction MutateDataProcessingImmAddSub::mutate_s(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        bool S = instruction.is_set(29);
        result.set_bit(29, !S);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_width(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        bool sf = instruction.is_set(31);
        result.set_bit(31, !sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_operator(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        bool op = instruction.is_set(30);
        result.set_bit(30, !op);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int reg = uniform_int(generator) % 32;
        auto result = instruction;
        result.set_range(0, 5, reg);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);

        auto def_ins = program.get_def_in(index);
        int reg = def_ins.random_gp(generator);

//        int reg = uniform_int(generator) % 32;
        auto result = instruction;
        result.set_range(5, 10, reg);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_imm12(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int index2 = uniform_int(generator) % 12;
        auto result = instruction;
        result.inverse_bit(10 + index2);
        return result;

        // int imm12 = uniform_int(generator) % (1 << 12);
        // auto result = instruction;
        // result.set_range(10, 22, imm12);
        // return result;
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate_sh(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sh = instruction.is_set(22);
        auto result = instruction;
        result.set_bit(22, !sh);
        return result;
    }

    MutateDataProcessingImmAddSub::MutateDataProcessingImmAddSub(std::mt19937 &generator, Prob prob)
        : dist{{ prob.w_s, prob.w_width, prob.w_operator, prob.w_rd, prob.w_rn, prob.w_imm12, prob.w_sh }},
          generator(generator)
    {}

    arm::Instruction MutateDataProcessingImmAddSub::mutate(const arm::Program& program, int index) {
        auto random_index = dist(generator);
        switch (random_index) {
            case 0:
                return mutate_s(program, index);
            case 1:
                return mutate_width(program, index);
            case 2:
                return mutate_operator(program, index);
            case 3:
                return mutate_rd(program, index);
            case 4:
                return mutate_rn(program, index);
            case 5:
                return mutate_imm12(program, index);
            case 6:
                return mutate_sh(program, index);
        }
        assert(false);
    }
}

// mutate logical imm
namespace megumin {
    arm::Instruction MutateDataProcessingImmLogical::mutate_opc(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        int opc = uniform_int(generator) % 4;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_n(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        int N = uniform_int(generator) % 2;
        int sf = result.is_set(31);
        if (sf == 0 && N == 1) {
            // this is undefined, return original instruction
            return result;
        }
        if (N == 0 && instruction.get_range(11, 16).as_u64() == 0b11111) {
            return result;
        }
        int level = uniform_int(generator) % (sf && N ? 6 : 5) + 1;
        result.set_range(10, 16, (0b11110ll << level) & (0b111111ll));
        result.set_range(10, 10 + level, uniform_int(generator) % ((1 << level) - 1));

        result.set_bit(22, N);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_sf(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        int sf = uniform_int(generator) % 2;
        int N = instruction.is_set(22);
        if (sf == 0 && N == 1) {
            return result;
        }
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_immr(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        // mutate rotations (immr)
        int r = uniform_int(generator) % (1 << 6);
        auto result = instruction;
        result.set_range(16, 22, r);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_imms(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        // mutate imms (element size and value)
        auto result = instruction;

        // assume N and sf are valid

        bool sf = instruction.is_set(31);
        bool N = instruction.is_set(22);

        int level = uniform_int(generator) % (sf && N ? 6 : 5) + 1;
        result.set_range(10, 16, (0b11110ll << level) & (0b111111ll));
        result.set_range(10, 10 + level, uniform_int(generator) % ((1 << level) - 1));
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    MutateDataProcessingImmLogical::MutateDataProcessingImmLogical(std::mt19937 &generator, Prob prob)
            : generator(generator),
              dist{{
                           prob.w_opc,
                           prob.w_n,
                           prob.w_sf,
                           prob.w_immr,
                           prob.w_imms,
                           prob.w_rn,
                           prob.w_rd,
                   }}
    {}

    arm::Instruction MutateDataProcessingImmLogical::mutate(const arm::Program& program, int index) {
        int index2 = dist(generator);
        switch (index2) {
            case 0:
                return mutate_opc(program, index);
            case 1:
                return mutate_n(program, index);
            case 2:
                return mutate_sf(program, index);
            case 3:
                return mutate_immr(program, index);
            case 4:
                return mutate_imms(program, index);
            case 5:
                return mutate_rn(program, index);
            case 6:
                return mutate_rd(program, index);
        }
        assert(false);
    }
}


// mutate move wide imm
namespace megumin {
    MutateDataProcessingImmMoveWide::MutateDataProcessingImmMoveWide(std::mt19937 &generator, Prob prob)
            : generator(generator),
              discrete{{
                               prob.w_sf,
                               prob.w_opc,
                               prob.w_hw,
                               prob.w_imm16,
                               prob.w_rd,
                       }}
    {}

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_sf(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sf = uniform_int(generator) % 2;
        if (sf == 0 && instruction.is_set(22)) {
            return instruction;
        }
        auto result = instruction;
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_opc(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int opc = uniform_int(generator) % 4;
        if (opc == 0b01) {
            opc = 0b11;
        }
        auto result = instruction;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_hw(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        bool sf = instruction.is_set(31);
        int hw;
        if (sf) {
            hw = uniform_int(generator) % 4;
        } else {
            hw = uniform_int(generator) % 2;
        }
        auto result = instruction;
        result.set_range(21, 23, hw);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_imm16(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int imm16 = uniform_int(generator) % (1 << 16);
        auto result = instruction;
        result.set_range(5, 21, imm16);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate(const arm::Program& program, int index) {
        int index2 = discrete(generator);
        switch (index2) {
            case 0:
                return mutate_sf(program, index);
            case 1:
                return mutate_opc(program, index);
            case 2:
                return mutate_hw(program, index);
            case 3:
                return mutate_imm16(program, index);
            case 4:
                return mutate_rd(program, index);
            default:
                assert(false);
        }
    }
}

// mutate bitfield
namespace megumin {
    MutateDataProcessingBitfield::MutateDataProcessingBitfield(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_sf_and_n,
              prob.w_opc,
              prob.w_immr,
              prob.w_imms,
              prob.w_rn,
              prob.w_rd,
          }}
    {}

    arm::Instruction MutateDataProcessingBitfield::mutate_sf_and_n(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sf = instruction.is_set(31);
        int sf2 = !sf;

        bool imms5 = instruction.is_set(15);
        bool immr5 = instruction.is_set(21);
        if (sf2 == 0 && (imms5 || immr5)) {
            return instruction;
        }

        auto result = instruction;
        result.set_bit(31, sf2);
        result.set_bit(22, sf2);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_opc(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int opc = uniform_int(generator) % 3;
        auto result = instruction;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_immr(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sf = instruction.is_set(31);
        int x;
        if (sf) {
            x = 1 << 6;
        } else {
            x = 1 << 5;
        }
        int r = uniform_int(generator) % x;
        auto result = instruction;
        result.set_range(16, 22, r);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_imms(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        bool sf = instruction.is_set(31);

        int x = sf ? (1 << 6) : (1 << 5);
        result.set_range(10, 16, uniform_int(generator) % x);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;

        auto def_ins = program.get_def_in(index);
        int reg = def_ins.random_gp(generator);

//        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        result.set_range(5, 10, reg);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate(const arm::Program& program, int index) {
        int index2 = discrete(generator);
        switch (index2) {
            case 0:
                return mutate_sf_and_n(program, index);
            case 1:
                return mutate_opc(program, index);
            case 2:
                return mutate_immr(program, index);
            case 3:
                return mutate_imms(program, index);
            case 4:
                return mutate_rn(program, index);
            case 5:
                return mutate_rd(program, index);
        }
        assert(false);
    }
}

// mutate extract
namespace megumin {
    MutateDataProcessingExtract::MutateDataProcessingExtract(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_sf,
              prob.w_rm,
              prob.w_imms,
              prob.w_rn,
              prob.w_rd,
          }}
    {}

    arm::Instruction MutateDataProcessingExtract::mutate_sf(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        bool sf = instruction.is_set(31);
        bool imms5 = instruction.is_set(15);
        bool sf2 = !sf;

        if (!sf2 && imms5) {
            return instruction;
        }

        auto result = instruction;
        result.set_bit(31, sf2);
        result.set_bit(22, sf2);
        return result;
    }

    arm::Instruction MutateDataProcessingExtract::mutate_rm(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(16, 21, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingExtract::mutate_imms(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        bool sf = instruction.is_set(31);
        int x = sf ? (1 << 6) : (1 << 5);
        auto result = instruction;
        result.set_range(10, 16, uniform_int(generator) % x);
        return result;
    }

    arm::Instruction MutateDataProcessingExtract::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingExtract::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingExtract::mutate(const arm::Program& program, int index) {
        int index2 = discrete(generator);
        switch (index2) {
            case 0:
                return mutate_sf(program, index);
            case 1:
                return mutate_rm(program, index);
            case 2:
                return mutate_imms(program, index);
            case 3:
                return mutate_rn(program, index);
            case 4:
                return mutate_rd(program, index);
        }
        assert(false);
    }
}
