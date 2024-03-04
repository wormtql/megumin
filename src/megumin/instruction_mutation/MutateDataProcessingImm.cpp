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
    MutateDataProcessingImmAddSub::MutateDataProcessingImmAddSub(Prob prob)
        : InstructionMutation({
            {make_mutate_bit(29), prob.w_s},
            {make_mutate_bit(31), prob.w_width},
            {make_mutate_bit(30), prob.w_operator},
            {mutate_rd, prob.w_rd},
            {mutate_rn, prob.w_rn},
            {make_mutate_range_bitwise(10, 22), prob.w_imm12},
            {make_mutate_bit(22), prob.w_sh}
        })
    {}
}

// mutate logical imm
namespace megumin {
    arm::Instruction MutateDataProcessingImmLogical::mutate_n(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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

    arm::Instruction MutateDataProcessingImmLogical::mutate_sf(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        auto result = instruction;
        int sf = uniform_int(generator) % 2;
        int N = instruction.is_set(22);
        if (sf == 0 && N == 1) {
            return result;
        }
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmLogical::mutate_imms(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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

    MutateDataProcessingImmLogical::MutateDataProcessingImmLogical(Prob prob)
            : InstructionMutation({
                {make_mutate_range(29, 31), prob.w_opc},
                {mutate_n, prob.w_n},
                {mutate_sf, prob.w_sf},
                {make_mutate_range(16, 22), prob.w_immr},
                {mutate_imms, prob.w_imms},
                {mutate_rn, prob.w_rn},
                {mutate_rd, prob.w_rd},
            })
    {}
}


// mutate move wide imm
namespace megumin {
    MutateDataProcessingImmMoveWide::MutateDataProcessingImmMoveWide(Prob prob)
            : InstructionMutation({
                {mutate_sf, prob.w_sf},
                {mutate_opc, prob.w_opc},
                {mutate_hw, prob.w_hw},
                {make_mutate_range_bitwise(5, 21), prob.w_imm16},
                {mutate_rd, prob.w_rd},
            })
    {}

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_sf(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        int sf = uniform_int(generator) % 2;
        if (sf == 0 && instruction.is_set(22)) {
            return instruction;
        }
        auto result = instruction;
        result.set_bit(31, sf);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_opc(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        int opc = uniform_int(generator) % 3;
        if (opc == 0b01) {
            opc = 0b11;
        }
        auto result = instruction;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingImmMoveWide::mutate_hw(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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
}

// mutate bitfield
namespace megumin {
    MutateDataProcessingBitfield::MutateDataProcessingBitfield(Prob prob)
        : InstructionMutation({
            {mutate_sf_and_n, prob.w_sf_and_n},
            {mutate_opc, prob.w_opc},
            {mutate_immr, prob.w_immr},
            {mutate_imms, prob.w_imms},
            {mutate_rn, prob.w_rn},
            {mutate_rd, prob.w_rd},
        })
    {}

    arm::Instruction MutateDataProcessingBitfield::mutate_sf_and_n(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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

    arm::Instruction MutateDataProcessingBitfield::mutate_opc(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        int opc = uniform_int(generator) % 3;
        auto result = instruction;
        result.set_range(29, 31, opc);
        return result;
    }

    arm::Instruction MutateDataProcessingBitfield::mutate_immr(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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

    arm::Instruction MutateDataProcessingBitfield::mutate_imms(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        auto result = instruction;
        bool sf = instruction.is_set(31);

        int x = sf ? (1 << 6) : (1 << 5);
        result.set_range(10, 16, uniform_int(generator) % x);
        return result;
    }
}

// mutate extract
namespace megumin {
    MutateDataProcessingExtract::MutateDataProcessingExtract(Prob prob)
        : InstructionMutation({
            {mutate_sf, prob.w_sf},
            {mutate_rm, prob.w_rm},
            {mutate_imms, prob.w_imms},
            {mutate_rn, prob.w_rn},
            {mutate_rd, prob.w_rd}
        })
    {}

    arm::Instruction MutateDataProcessingExtract::mutate_sf(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
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

    arm::Instruction MutateDataProcessingExtract::mutate_imms(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        bool sf = instruction.is_set(31);
        int x = sf ? (1 << 6) : (1 << 5);
        auto result = instruction;
        result.set_range(10, 16, uniform_int(generator) % x);
        return result;
    }
}
