//
// Created by 58413 on 2023/6/27.
//

#include <z3++.h>
#include "InstructionExecutionS.h"
#include "ArmUtilsS.h"
#include "ArmUtils.h"
#include "megumin_utils.h"

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

    void InstructionExecutionS::visit_dp_imm_logical(const Instruction &instruction) {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bool N = instruction.is_set(22);

        bits immr = instruction.get_range(16, 22);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int datasize = sf ? 64 : 32;
        bits imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;

        if (opc == 0b00) {
            // and
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.get_gp(datasize, n, false, true);
            auto& c = operand1.ctx();
            expr result = operand1 & c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, true);
        } else if (opc == 0b01) {
            // orr
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 | c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, true);
        } else if (opc == 0b10) {
            // eor
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 ^ c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, true);
        } else if (opc == 0b11) {
            // ands
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 & c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, true);

            PStateS p_state{
                result.extract(datasize - 1, datasize - 1) == 1,
                result == 0,
                c.bool_val(false),
                c.bool_val(false),
            };

            state.p_state.set_nzcv(p_state);
        }
    }

    void InstructionExecutionS::visit_dp_imm_move_wide(const Instruction &instruction) {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bits hw = instruction.get_range(21, 23);
        bits imm16 = instruction.get_range(5, 21);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();

        int datasize = sf ? 64 : 32;
        if (!sf && hw.is_set(1)) {
            megumin::megumin_assert(false);
        }

        bits pos_bits = hw.concat(bits{4, 0});
        int pos = pos_bits.as_u32();

        auto& context = state.sp.ctx();

        if (opc == 0b00) {
            // movn
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            result = ~result;
            state.set_gp(datasize, d, context.bv_val(result.data0, datasize), false);
        } else if (opc == 0b10) {
            // movz
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            state.set_gp(datasize, d, context.bv_val(result.data0, datasize), false);
        } else if (opc == 0b11) {
            // movk
            expr result = state.get_gp(datasize, d, false, true);

            result = megumin::set_expr_range(result, pos, pos + 16, imm16.as_i64());
            state.set_gp(datasize, d, result, false);
        } else {
            megumin::megumin_assert(false);
        }
    }
}

