//
// Created by 58413 on 2023/4/22.
//

#include <Bitvec.h>
#include "MutateDataProcessingReg.h"
#include "megumin_utils.h"

using arm::bits;

// mutate 2-source
namespace megumin {
    arm::Instruction MutateDataProcessingReg2Source::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
//        int temp = uniform_int(generator);
        bits opcode = instruction.get_range(10, 16);
        if (opcode.get_range(2, 6) == 0) {
            result.set_range(10, 16, 0b10 + uniform_int(generator) % 2);
        } else {
            result.set_range(10, 16, 0b1000 + uniform_int(generator) % 4);
        }
        return result;
    }

    arm::Instruction MutateDataProcessingReg2Source::mutate_sf(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        bool sf = instruction.is_set(31);
        auto result = instruction;
        result.set_bit(31, !sf);
        return result;
    }

    arm::Instruction MutateDataProcessingReg2Source::mutate_rm(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(16, 21, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingReg2Source::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(5, 10, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingReg2Source::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction MutateDataProcessingReg2Source::mutate(const arm::Program& program, int index) {
        int index2 = discrete(generator);
        switch (index2) {
            case 0:
                return mutate_opcode(program, index);
            case 1:
                return mutate_sf(program, index);
            case 2:
                return mutate_rm(program, index);
            case 3:
                return mutate_rn(program, index);
            case 4:
                return mutate_rd(program, index);
        }
        megumin_assert(false);
    }

    MutateDataProcessingReg2Source::MutateDataProcessingReg2Source(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_opcode,
              prob.w_sf,
              prob.w_rm,
              prob.w_rn,
              prob.w_rd
          }}
    {}
}

// mutate 1-source
namespace megumin {
    arm::Instruction MutateDataProcessingReg1Source::mutate_sf(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sf = instruction.is_set(31);
        int sf2 = !sf;
        bits opcode = instruction.get_range(10, 16);
        if (opcode == 0b000011 && sf2 == 0) {
            auto result = instruction;
            result.set_bit(31, sf2);
            result.set_range(10, 16, uniform_int(generator) % 3);
            return result;
        }

        auto result = instruction;
        result.set_bit(31, sf2);
        return result;
    }

    arm::Instruction MutateDataProcessingReg1Source::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        int sf = instruction.is_set(31);

        int opcode;
        int temp = uniform_int(generator) % 6;
        if (temp < 4) {
            opcode = temp;
        } else {
            opcode = 0b100 + temp - 4;
        }

        if (opcode == 0b000011 && sf == 0) {
            auto result = instruction;
            result.set_bit(31, true);
            result.set_range(10, 16, opcode);
            return result;
        }

        auto result = instruction;
        result.set_range(10, 16, opcode);
        return result;
    }

    arm::Instruction MutateDataProcessingReg1Source::mutate_rn(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        return InstructionMutation::mutate_rn(uniform_int(generator), instruction);
    }

    arm::Instruction MutateDataProcessingReg1Source::mutate_rd(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        return InstructionMutation::mutate_rd(uniform_int(generator), instruction);
    }

    arm::Instruction MutateDataProcessingReg1Source::mutate(const arm::Program& program, int index) {
        int index2 = discrete(generator);
        switch (index2) {
            case 0:
                return mutate_sf(program, index);
            case 1:
                return mutate_opcode(program, index);
            case 2:
                return mutate_rn(program, index);
            case 3:
                return mutate_rd(program, index);
        }
        megumin_assert(false);
    }

    MutateDataProcessingReg1Source::MutateDataProcessingReg1Source(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_sf,
              prob.w_opcode,
              prob.w_rn,
              prob.w_rd
          }}
    {}
}