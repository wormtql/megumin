//
// Created by 58413 on 2023/4/17.
//

#include "InstructionMutation.h"

namespace megumin {
    arm::Instruction InstructionMutation::mutate_rn(int value, const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(5, 10, value % (1 << 5));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rd(int value, const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, value % (1 << 5));
        return result;
    }
}
