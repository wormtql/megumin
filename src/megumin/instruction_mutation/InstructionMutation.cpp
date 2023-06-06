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

    arm::Instruction InstructionMutation::mutate_sf(int value, const arm::Instruction &instruction) {
        auto result = instruction;
        result.get_bits_mut().inverse_bit(31);
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rm(const arm::Instruction &instruction, std::mt19937 &generator) {
        auto result = instruction;
        std::uniform_int_distribution<> uniform_int;
        result.set_range(16, 21, uniform_int(generator) % (1 << 5));
        return result;
    }
}
