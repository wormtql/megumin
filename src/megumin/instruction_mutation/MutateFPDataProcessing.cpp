//
// Created by 58413 on 2023/5/11.
//

#include <array>
#include "MutateFPDataProcessing.h"
#include "megumin_utils.h"

namespace megumin {
    MutateFPDataProcessing1::MutateFPDataProcessing1(Prob prob)
        : InstructionMutation({{
            {mutate_fp_ptype, prob.w_ptype},
            {mutate_opcode, prob.w_opcode},
            {mutate_rn_fp, prob.w_rn},
            {mutate_rd_fp, prob.w_rd}
        }})
    {}

    arm::Instruction MutateFPDataProcessing1::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        static int opcodes[] = {
                0b000000, // fmov
                0b000001, // fabs
                0b000010, // fneg
                0b000011, // fsqrt
                0b000101, // fcvt
//                0b000111, // fcvt single to half
                0b001000, // frintn
                0b001001, // frintp
                0b001010, // frintm
                0b001011, // frintz
                0b001100, // frinta
//                0b001110, // frintx
//                0b001111, // frinti
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
            {mutate_fp_ptype, prob.w_ptype},
            {mutate_rm_fp, prob.w_rm},
            {mutate_opcode, prob.w_opcode},
            {mutate_rn_fp, prob.w_rn},
            {mutate_rd_fp, prob.w_rd},
        })
    {}

    arm::Instruction MutateFPDataProcessing2::mutate_opcode(const arm::Program& program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        static int opcodes[] = {
                0b0000,     // fmul
                0b0001,     // fdiv
                0b0010,     // fadd
                0b0011,     // fsub
                0b0100,     // fmax
                0b0101,     // fmin
                0b0110,     // fmaxnm
                0b0111,     // fminnm
                0b1000,     // fnmul
        };
        int opcode_size = std::end(opcodes) - std::begin(opcodes);
        int opcode = uniform_int(generator) % opcode_size;

        auto result = instruction;
        result.set_range(12, 16, opcode);
        return result;
    }
}

// fp dp 3-source
namespace megumin {
    MutateFPDataProcessing3::MutateFPDataProcessing3(Prob prob)
        : InstructionMutation({
            {mutate_fp_ptype, prob.w_ptype},
            {mutate_opcode, prob.w_op},
            {mutate_rm_fp, prob.w_op},
            {mutate_ra_fp, prob.w_ra},
            {mutate_rn_fp, prob.w_rn},
            {mutate_rd_fp, prob.w_rd},
        })
    {}

    arm::Instruction MutateFPDataProcessing3::mutate_opcode(const arm::Program &program, int index) {
        const arm::Instruction& instruction = program.get_instruction_const(index);
        auto result = instruction;

        result.set_bit(21, uniform_int(generator) % 2);
        result.set_bit(15, uniform_int(generator) % 2);

        return result;
    }
}

// fp imm
namespace megumin {
    MutateFPImm::MutateFPImm(Prob prob)
        : InstructionMutation({
            {mutate_fp_ptype, prob.w_ptype},
            {make_mutate_range(13, 21), prob.w_imm8},
            {mutate_rd_fp, prob.w_rd}
        })
    {}
}
