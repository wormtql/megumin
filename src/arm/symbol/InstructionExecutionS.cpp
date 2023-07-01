//
// Created by 58413 on 2023/6/27.
//

#include <z3++.h>
#include "InstructionExecutionS.h"
#include "ArmUtilsS.h"
#include "ArmUtils.h"
#include "megumin_utils.h"

using megumin::megumin_assert;

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

    void InstructionExecutionS::visit_dp_imm_bitfield(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bits immr = instruction.get_range(16, 22);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();

        int datasize = sf ? 64 : 32;

        bits wmask{datasize, 0};
        bits tmask{datasize, 0};

        megumin_assert(!(sf && !N));
        megumin_assert(!(!sf && (N || immr.is_set(5) || imms.is_set(5))));

        auto R = immr.as_i32();
        auto S = imms.as_i32();
        auto temp = ArmUtils::decode_bit_mask(datasize, N, imms, immr, false);
        wmask = temp.first;
        tmask = temp.second;

        auto& c = state.sp.ctx();

        expr wmask_val = c.bv_val(wmask.data0, datasize);
        expr tmask_val = c.bv_val(tmask.data0, datasize);

        if (opc == 0b00) {
            // sbfm
            expr src = state.get_gp(datasize, n, false, true);
            expr bot = ArmUtilSharedFunctionsS::ror(src, R) & wmask_val;
            expr top = ArmUtilsS::replicate(src.extract(S, S), datasize);
            expr result = (top & ~tmask_val) | (bot & tmask_val);
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b01) {
            // bfm
            expr dst = state.get_gp(datasize, d, false, true);
            expr src = state.get_gp(datasize, n, false, true);
            expr bot = (dst & ~wmask_val) | (ArmUtilSharedFunctionsS::ror(src, R) & wmask_val);
            expr result = (dst & ~tmask_val) | (bot & tmask_val);
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b10) {
            // ubfm
            expr src = state.get_gp(datasize, n, false, true);
            expr bot = ArmUtilSharedFunctionsS::ror(src, R) & wmask_val;
            state.set_gp(datasize, d, bot & tmask_val, false);
        }
    }

    void InstructionExecutionS::visit_dp_imm_extract(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits op21 = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bool o0 = instruction.is_set(21);
        bits rm = instruction.get_range(16, 21);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        int datasize = sf ? 64 : 32;

        if (op21 == 0b00 && !o0) {
            // extr
            megumin_assert(N == sf);
            if (sf == 0 && imms.is_set(5)) {
                megumin_assert(false);
            }

            unsigned int lsb = imms.as_u32();
            expr operand1 = state.get_gp(datasize, n, false, true);
            expr operand2 = state.get_gp(datasize, m, false, true);

            if (lsb == 0) {
                state.set_gp(datasize, d, operand2, false);
            } else {
                expr low = operand2.extract(datasize - 1, lsb);
                expr high = operand1.extract(lsb - 1, 0);
                expr result = z3::concat(low, high);
                state.set_gp(datasize, d, result, false);
            }
        } else {
            megumin::megumin_assert(false);
        }
    }
}

