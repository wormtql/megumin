//
// Created by 58413 on 2023/5/11.
//

#include <array>
#include "MutateFPDataProcessing.h"
#include "megumin_utils.h"

namespace megumin {
    MutateFPDataProcessing1::MutateFPDataProcessing1(Prob prob)
        : InstructionMutation({{
            {mutate_ptype, prob.w_ptype},
            {mutate_opcode, prob.w_opcode},
            {mutate_rn_fp, prob.w_rn},
            {mutate_rd_fp, prob.w_rd}
        }})
    {}

    arm::Instruction MutateFPDataProcessing1::mutate_ptype(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        // todo half precision
        result.set_range(22, 24, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction MutateFPDataProcessing1::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
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
}

// fp dp 2-source
namespace megumin {
    MutateFPDataProcessing2::MutateFPDataProcessing2(Prob prob)
        : InstructionMutation({
            {mutate_ptype, prob.w_ptype},
            {mutate_rm_fp, prob.w_rm},
            {mutate_opcode, prob.w_opcode},
            {mutate_rn_fp, prob.w_rn},
            {mutate_rd_fp, prob.w_rd},
        })
    {}

    arm::Instruction MutateFPDataProcessing2::mutate_ptype(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;
        // todo half precision
        result.set_range(22, 24, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction MutateFPDataProcessing2::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
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
}
