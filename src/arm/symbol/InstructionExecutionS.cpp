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
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b01) {
            // orr
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 | c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b10) {
            // eor
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 ^ c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b11) {
            // ands
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            expr operand1 = state.gp.get(datasize, n);
            auto& c = operand1.ctx();
            expr result = operand1 & c.bv_val(imm.data0, datasize);
            state.set_gp(datasize, d, result, false);

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

    void InstructionExecutionS::visit_dp_reg_1source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits opcode2 = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(10, 16);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        int n = rn.as_i32();
        int d = rd.as_i32();
        int datasize = sf ? 64 : 32;

        megumin_assert(S == 0);

        if (opcode2 == 0 && opcode == 0) {
            // rbit
            expr operand = state.get_gp(datasize, n, false, true);
            expr result = megumin::reverse_expr(operand);
            state.set_gp(datasize, d, result, false);
        } else if (opcode2 == 0 && (opcode >> 2) == 0) {
            // rev
//            megumin::megumin_todo();
            bits opc = instruction.get_range(10, 12);
            int container_size = 0;
            if (opc == 0b01) {
                container_size = 16;
            } else if (opc == 0b10) {
                container_size = 32;
            } else if (opc == 0b11) {
                megumin_assert(sf == 1);
                container_size = 64;
            } else {
                megumin_assert(false);
            }

            expr operand = state.get_gp(datasize, n, false, true);

            int containers = datasize / container_size;
            megumin_assert(containers >= 1);
            int elements_per_container = container_size / 8;
            int index = 0;
            int rev_index;

            expr result = operand;

            for (int c = 0; c < containers; c++) {
                rev_index = index + ((elements_per_container - 1) * 8);
                for (int e = 0; e < elements_per_container; e++) {
                    megumin_assert(rev_index <= datasize);
                    megumin_assert(rev_index + 8 <= datasize);
                    megumin_assert(index <= datasize);
                    megumin_assert(index + 8 <= datasize);

                    expr ext = operand.extract(index + 7, index);
                    result = megumin::set_expr_range(result, rev_index, rev_index + 8, ext);
                    index += 8;
                    rev_index -= 8;
                }
            }

            state.set_gp(datasize, d, result, false);
        } else if (opcode2 == 0 && opcode == 0b000100) {
            // clz

            expr operand1 = state.gp.get(datasize, n);
            expr result = megumin::count_leading_zero(datasize, operand1);
            state.set_gp(datasize, d, result, false);
        } else if (opcode2 == 0 && opcode == 0b000101) {
            // cls
            expr operand1 = state.get_gp(datasize, n, false, true);
            expr result = megumin::count_leading_signed(operand1, datasize);
            megumin::megumin_assert(result.is_bv());
            state.set_gp(datasize, d, result, false);
//            bits operand1 = state.gp.get(datasize, n);
//            int result = ArmUtilsSharedFunctions::count_leading_sign_bits(operand1);
//            state.gp.set(datasize, d, bits{datasize, result});
        }
        else {
            megumin_assert(false);
        }
    }

    void InstructionExecutionS::visit_dp_reg_2source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_range(16, 21);
        bits opc = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        int datasize = sf ? 64 : 32;

        auto& c = state.sp.ctx();

        if (S == 0) {
            if (opc == 0b000010) {
                // udiv
                expr operand1 = state.get_gp(datasize, n, false, true);
                expr operand2 = state.get_gp(datasize, m, false, true);

                expr result = z3::udiv(operand1, operand2);
                result = z3::ite(operand2 == 0, c.bv_val(0, datasize), result);
                state.set_gp(datasize, d, result, false);
            } else if (opc == 0b000011) {
                // sdiv
                expr operand1 = state.get_gp(datasize, n, false, true);
                expr operand2 = state.get_gp(datasize, m, false, true);

                expr result = operand1 / operand2;
                result = z3::ite(
                        operand2 == 0,
                        c.bv_val(0, datasize),
                        z3::ite(
                                z3::bvsdiv_no_overflow(operand1, operand2),
                                result,
                                c.bv_val(0, datasize)
                                )
                        );
                state.set_gp(datasize, d, result, false);
            } else if ((opc >> 2) == 0b10) {
                // lslv/lsrv/asrv/rorv
                int shift_type = instruction.get_range(10, 12).as_i32();
                expr operand1 = state.get_gp(datasize, n, false, true);
                expr operand2 = state.get_gp(datasize, m, false, true);

                expr result = ArmUtilsS::shift_reg(operand1, shift_type, operand2);
                state.set_gp(datasize, d, result, false);
            } else {
                megumin::megumin_unreachable();
            }
        } else {
            megumin::megumin_unreachable();
        }
    }

    void InstructionExecutionS::visit_dp_reg_logical_shifted_reg(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bits shift = instruction.get_range(22, 24);
        bool N = instruction.is_set(21);
        bits rm = instruction.get_rm();
        bits imm6 = instruction.get_range(10, 16);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        int datasize = sf ? 64 : 32;
        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();

        megumin::megumin_assert(!(!sf && imm6[5] == 1));
        int shift_amount = imm6.as_i32();

        expr operand1 = state.get_gp(datasize, n, false, true);
        expr operand2 = ArmUtilsS::shift_reg(state.get_gp(datasize, m, false, true), shift.as_i32(), shift_amount);
        if (N) {
            operand2 = ~operand2;
        }

        if (opc == 0b00) {
            // and bic
            expr result = operand1 & operand2;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b01) {
            // orr orn
            expr result = operand1 | operand2;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b10) {
            // eor eon
            expr result = operand1 ^ operand2;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b11) {
            // ands bics
            expr result = operand1 & operand2;
            auto& c = result.ctx();

            PStateS p_state{
                result.extract(datasize - 1, datasize - 1) == 1,
                result == 0,
                c.bool_val(false),
                c.bool_val(false)
            };

            state.set_gp(datasize, d, result, false);
            state.p_state.set_nzcv(p_state);
        }
    }

    void InstructionExecutionS::visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits shift = instruction.get_range(22, 24);
        bits rm = instruction.get_range(16, 21);
        bits imm6 = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        int datasize = sf ? 64 : 32;
        int shift_amount = imm6.as_i32();

        megumin::megumin_assert(shift != 0b11);
        megumin::megumin_assert(sf != 0 || imm6[5] != 1);

        expr operand1 = state.get_gp(datasize, n, false, true);
        expr operand2 = ArmUtilsS::shift_reg(state.get_gp(datasize, m, false, true), shift.as_i32(), shift_amount);

        if (op == 0) {
            // add adds
            auto result = ArmUtilsS::add_with_carry(operand1, operand2, false);
            state.set_gp(datasize, d, result.first, false);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else if (op == 1) {
            // sub subs
            operand2 = ~operand2;
            auto result = ArmUtilsS::add_with_carry(operand1, operand2, true);
            state.set_gp(datasize, d, result.first, false);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void InstructionExecutionS::visit_dp_reg_add_sub_with_carry(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_rm();
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        int d = rd.as_i32();
        int m = rm.as_i32();
        int n = rn.as_i32();
        int datasize = sf ? 64 : 32;

        expr operand1 = state.get_gp(datasize, n, false, true);
        expr operand2 = state.get_gp(datasize, m, false, true);

        if (op == 0) {
            // adc/adcs
            expr carry = megumin::z3_bool_to_bv(state.p_state.c, datasize);
            auto result = ArmUtilsS::add_with_carry(operand1, operand2, carry);
            state.set_gp(datasize, d, result.first, false);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else if (op == 1) {
            // sbc/sbcs
            operand2 = ~operand2;
            expr carry = megumin::z3_bool_to_bv(state.p_state.c, datasize);
            auto result = ArmUtilsS::add_with_carry(operand1, operand2, carry);
            state.set_gp(datasize, d, result.first, false);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void InstructionExecutionS::visit_dp_reg_cond_select(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_range(16, 21);
        bits cond = instruction.get_range(12, 16);
        bits op2 = instruction.get_range(10, 12);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int m = rm.as_i32();
        int n = rn.as_i32();
        int datasize = sf ? 64 : 32;

        if (op == 0 && op2 == 0b00) {
            // csel
            expr condition_holds = ArmUtilSharedFunctionsS::condition_holds(cond.as_i32(), state);

            expr result = z3::ite(
                    condition_holds,
                    state.get_gp(datasize, n, false, true),
                    state.get_gp(datasize, m, false, true)
                    );
            state.set_gp(datasize, d, result, false);
        } else if (op == 0 && op2 == 0b01) {
            // csinc
            expr condition_holds = ArmUtilSharedFunctionsS::condition_holds(cond.as_i32(), state);

            expr result = z3::ite(
                    condition_holds,
                    state.get_gp(datasize, n, false, true),
                    state.get_gp(datasize, m, false, true) + 1
                    );
            state.set_gp(datasize, d, result, false);
        } else if (op == 1 && op2 == 0b00) {
            // csinv
            expr condition_holds = ArmUtilSharedFunctionsS::condition_holds(cond.as_i32(), state);

            expr result = z3::ite(
                    condition_holds,
                    state.get_gp(datasize, n, false, true),
                    ~state.get_gp(datasize, m, false, true)
            );
            state.set_gp(datasize, d, result, false);
        } else if (op == 1 && op2 == 0b01) {
            // csneg
            expr condition_holds = ArmUtilSharedFunctionsS::condition_holds(cond.as_i32(), state);

            expr result = z3::ite(
                    condition_holds,
                    state.get_gp(datasize, n, false, true),
                    ~state.get_gp(datasize, m, false, true) + 1
            );
            state.set_gp(datasize, d, result, false);
        }
    }
}
