//
// Created by 58413 on 2023/4/22.
//

#include <functional>

#include <Bitvec.h>
#include "MutateDataProcessingReg.h"
#include "megumin_utils.h"

using arm::bits;
using namespace std::placeholders;

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

    MutateDataProcessingReg2Source::MutateDataProcessingReg2Source(Prob prob)
        : InstructionMutation({
            {mutate_opcode, prob.w_opcode},
            {make_mutate_bit(31), prob.w_sf},
            {mutate_rm, prob.w_rm},
            {mutate_rn, prob.w_rn},
            {mutate_rd, prob.w_rd},
        })
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

    MutateDataProcessingReg1Source::MutateDataProcessingReg1Source(Prob prob)
        : InstructionMutation({
            {mutate_sf, prob.w_sf},
            {mutate_opcode, prob.w_opcode},
            {mutate_rn, prob.w_rn},
            {mutate_rd, prob.w_rd}
        })
    {}
}

// mutate logical shifted reg
namespace megumin {
    MutateDataProcessingRegLogical::MutateDataProcessingRegLogical(Prob p)
        : InstructionMutation({
            {mutate_sf, p.w_sf},
            {make_mutate_range(29, 31), p.w_opc},
            {make_mutate_range(22, 24), p.w_shift},
            {make_mutate_bit(21), p.w_N},
            {mutate_rm, p.w_rm},
            {mutate_imm6, p.w_imm6},
            {mutate_rn, p.w_rn},
            {mutate_rd, p.w_rd},
        })
    {}

    arm::Instruction MutateDataProcessingRegLogical::mutate_imm6(const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        if (result.is_set(31)) {
            result.set_range(10, 16, uniform_int(generator) % (1 << 6));
        } else {
            result.set_range(10, 16, uniform_int(generator) % (1 << 5));
        }
        return result;
    }

    arm::Instruction MutateDataProcessingRegLogical::mutate_sf(const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        if (result.is_set(15)) {
            return result;
        }
        result.inverse_bit(31);
        return result;
    }
}
