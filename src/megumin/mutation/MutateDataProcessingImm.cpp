//
// Created by 58413 on 2023/4/17.
//

#include <cstdlib>

#include "MutateDataProcessingImm.h"

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
}
