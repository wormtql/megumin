//
// Created by 58413 on 2023/4/17.
//

#include "Mutation.h"

namespace megumin {
    arm::Instruction Mutation::mutate_rn(int value, const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(5, 10, value % (1 << 5));
        return result;
    }

    arm::Instruction Mutation::mutate_rd(int value, const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, value % (1 << 5));
        return result;
    }
}
