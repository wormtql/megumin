//
// Created by 58413 on 2023/6/27.
//

#include "InstructionExecutionS.h"
#include "ArmUtilsS.h"

namespace arm {
    void InstructionExecutionS::visit_dp_imm_add_sub(const arm::Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool sh = instruction.is_set(22);
        bool S = instruction.is_set(29);
        bits imm12 = instruction.get_range(10, 22);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int datasize = sf ? 64 : 32;
        int d = rd.as_i32();
        int n = rn.as_i32();

        if (!op) {
            // add
            expr operand1 = state.get_gp(datasize, rn.as_i32(), true, false);
            bits imm = sh ? (imm12.zero_extend(datasize) << 12) : imm12.zero_extend(datasize);
            auto result = ArmUtilsS::add_with_carry(operand1, imm, false);
            state.set_gp(datasize, rd.as_i32(), result.first, true);

            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else {
            // sub
            expr operand1 = state.get_gp(datasize, n, true, false);
            bits imm = sh ? (imm12.zero_extend(datasize) << 12) : imm12.zero_extend(datasize);
            bits operand2 = ~imm;
            auto result = ArmUtilsS::add_with_carry(operand1, operand2, true);
            state.set_gp(datasize, d, result.first, true);

            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }
}

