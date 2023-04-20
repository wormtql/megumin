//
// Created by 58413 on 2023/4/17.
//

#include <cstdlib>

#include <ArmUtils.h>
#include <Bitvec.h>
#include "MutateDataProcessingImm.h"

using arm::bits;

namespace megumin {
    arm::Instruction MutateDataProcessingImmAddSubS::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        bool S = instruction.get_bit(29);
        result.set_bit(29, !S);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubWidth::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        bool sf = instruction.get_bit(31);
        result.set_bit(31, !sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubOperator::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        bool op = instruction.get_bit(30);
        result.set_bit(30, !op);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubRd::mutate(const arm::Instruction &instruction) {
        int reg = rand() % 32;
        auto result = instruction;
        result.set_range(0, 5, reg);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubRn::mutate(const arm::Instruction &instruction) {
        int reg = rand() % 32;
        auto result = instruction;
        result.set_range(5, 10, reg);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubImm12::mutate(const arm::Instruction &instruction) {
        int imm12 = rand() % (1 << 12);
        auto result = instruction;
        result.set_range(10, 22, imm12);
        return result;
    }

    arm::Instruction MutateDataProcessingImmAddSubSh::mutate(const arm::Instruction &instruction) {
        int sh = instruction.get_bit(22);
        auto result = instruction;
        result.set_bit(22, !sh);
        return result;
    }

    MutateDataProcessingImmAddSub::MutateDataProcessingImmAddSub(std::mt19937 &generator, Prob prob)
        : dist{{ prob.w_s, prob.w_width, prob.w_operator, prob.w_rd, prob.w_rn, prob.w_imm12, prob.w_sh }},
          generator(generator)
    {
        mutations.push_back(new MutateDataProcessingImmAddSubS());
        mutations.push_back(new MutateDataProcessingImmAddSubWidth());
        mutations.push_back(new MutateDataProcessingImmAddSubOperator());
        mutations.push_back(new MutateDataProcessingImmAddSubRd());
        mutations.push_back(new MutateDataProcessingImmAddSubRn());
        mutations.push_back(new MutateDataProcessingImmAddSubImm12());
        mutations.push_back(new MutateDataProcessingImmAddSubSh());
    }

    MutateDataProcessingImmAddSub::~MutateDataProcessingImmAddSub() {
        for (Mutation* m: mutations) {
            delete m;
        }
    }

    arm::Instruction MutateDataProcessingImmAddSub::mutate(const arm::Instruction &instruction) {
        auto random_index = dist(generator);
        Mutation* mutate = mutations[random_index];

        return mutate->mutate(instruction);
    }

    arm::Instruction MutateDataProcessingImmLogicalOpc::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        int opc = rand() % 4;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalN::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        int N = rand() % 2;
        int sf = result.get_bit(31);
        if (sf == 0 && N == 1) {
            // this is undefined, return original instruction
            return result;
        }
        if (N == 0 && instruction.get_range(11, 16).as_u64() == 0b11111) {
            return result;
        }
        result.set_bit(22, N);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalSf::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        int sf = rand() % 2;
        int N = instruction.get_bit(22);
        if (sf == 0 && N == 1) {
            return result;
        }
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalImmr::mutate(const arm::Instruction &instruction) {
        // mutate rotations (immr)
        int r = rand() % (1 << 6);
        auto result = instruction;
        result.set_range(16, 22, r);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalImms::mutate(const arm::Instruction &instruction) {
        // mutate imms (element size and value)
        auto result = instruction;

        // assume N and sf are valid

        bool sf = instruction.get_bit(31);
        bool N = instruction.get_bit(22);

        int level = rand() % (sf && N ? 6 : 5) + 1;
        result.set_range(10, 16, (0b11110 << level) & (0b111111));
        result.set_range(10, 10 + level, rand() % ((1 << level) - 1));
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalRn::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(5, 10, rand() % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogicalRd::mutate(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, rand() % (1 << 5));
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
    {
        mutations.push_back(new MutateDataProcessingImmLogicalOpc());
        mutations.push_back(new MutateDataProcessingImmLogicalN());
        mutations.push_back(new MutateDataProcessingImmLogicalSf());
        mutations.push_back(new MutateDataProcessingImmLogicalImmr());
        mutations.push_back(new MutateDataProcessingImmLogicalImms());
        mutations.push_back(new MutateDataProcessingImmLogicalRn());
        mutations.push_back(new MutateDataProcessingImmLogicalRd());
    }

    MutateDataProcessingImmLogical::~MutateDataProcessingImmLogical() {
        for (Mutation* m: mutations) {
            delete m;
        }
    }

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

    arm::Instruction MutateDataProcessingImmLogical::mutate(const arm::Instruction &instruction) {
        int index = dist(generator);
        auto m = mutations[index];
        return m->mutate(instruction);
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_sf(const arm::Instruction &instruction) {
        int sf = uniform_int(generator) % 2;
        if (sf == 0 && instruction.get_bit(22)) {
            return instruction;
        }
        auto result = instruction;
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_opc(const arm::Instruction &instruction) {
        int opc = uniform_int(generator) % 4;
        if (opc == 0b01) {
            opc = 0b11;
        }
        auto result = instruction;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_hw(const arm::Instruction &instruction) {
        bool sf = instruction.get_bit(31);
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

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_imm16(const arm::Instruction &instruction) {
        int imm16 = uniform_int(generator) % (1 << 16);
        auto result = instruction;
        result.set_range(5, 21, imm16);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_rd(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate(const arm::Instruction &instruction) {
        int index = discrete(generator);
        switch (index) {
            case 0:
                return mutate_sf(instruction);
            case 1:
                return mutate_opc(instruction);
            case 2:
                return mutate_hw(instruction);
            case 3:
                return mutate_imm16(instruction);
            case 4:
                return mutate_rd(instruction);
            default:
                assert(false);
        }
    }
}
