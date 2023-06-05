//
// Created by 58413 on 2023/5/11.
//

#include <array>
#include "MutateFPDataProcessing.h"

namespace megumin {
    MutateFPDataProcessing1::MutateFPDataProcessing1(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_ptype,
              prob.w_opcode,
              prob.w_rn,
              prob.w_rd
          }}
    {}

    arm::Instruction MutateFPDataProcessing1::mutate_ptype(const arm::Instruction &instruction) {
        auto result = instruction;
        // todo half precision
        result.set_range(22, 24, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction MutateFPDataProcessing1::mutate_opcode(const arm::Instruction &instruction) {
        static int opcodes[] = {
                0b000000,
                0b000001,
//                0b000010,
//                0b000011,
//                0b000101,
//                0b000111,
        };
        int opcode_size = std::end(opcodes) - std::begin(opcodes);
        int opcode = uniform_int(generator) % opcode_size;

        auto result = instruction;
        result.set_range(15, 21, opcode);
        return result;
    }

    arm::Instruction MutateFPDataProcessing1::mutate_rn(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateFPDataProcessing1::mutate_rd(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateFPDataProcessing1::mutate(const arm::Instruction &instruction) {
        int index = discrete(generator);
        switch (index) {
            case 0:
                return mutate_ptype(instruction);
            case 1:
                return mutate_opcode(instruction);
            case 2:
                return mutate_rn(instruction);
            case 3:
                return mutate_rd(instruction);
            default:
                assert(false);
        }
        assert(false);
        return instruction;
    }

}

// fp dp 2-source
namespace megumin {
    MutateFPDataProcessing2::MutateFPDataProcessing2(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_ptype,
              prob.w_rm,
              prob.w_opcode,
              prob.w_rn,
              prob.w_rd
          }}
    {}

    arm::Instruction MutateFPDataProcessing2::mutate_ptype(const arm::Instruction &instruction) {
        auto result = instruction;
        // todo half precision
        result.set_range(22, 24, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate_rm(const arm::Instruction& instruction) {
        auto result = instruction;
        result.set_range(16, 21, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate_opcode(const arm::Instruction &instruction) {
        // todo
        static int opcodes[] = {
                0b0010,
                0b0011,
        };
        int opcode_size = std::end(opcodes) - std::begin(opcodes);
        int opcode = uniform_int(generator) % opcode_size;

        auto result = instruction;
        result.set_range(12, 16, opcode);
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate_rn(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate_rd(const arm::Instruction &instruction) {
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate(const arm::Instruction &instruction) {
        int index = discrete(generator);
        switch (index) {
            case 0:
                return mutate_ptype(instruction);
            case 1:
                return mutate_rm(instruction);
            case 2:
                return mutate_opcode(instruction);
            case 3:
                return mutate_rn(instruction);
            case 4:
                return mutate_rd(instruction);
            default:
                assert(false);
        }
        assert(false);
        return instruction;
    }
}