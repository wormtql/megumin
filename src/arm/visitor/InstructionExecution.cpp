//
// Created by 58413 on 2023/6/2.
//

#include "InstructionExecution.h"
#include "ArmUtils.h"
#include "megumin_utils.h"
#include "MyFloat.h"
#include <absl/numeric/int128.h>
#include "FPUtils.h"

using megumin::megumin_assert;
using megumin::megumin_todo;

namespace arm {
    InstructionExecution::InstructionExecution(MachineState &state)
        :state(state)
    {}

    void InstructionExecution::visit_dp_imm_add_sub(const Instruction &instruction) {
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
            bits operand1 = state.get_gp(datasize, rn.as_i32(), true, false);
            bits imm = sh ? (imm12.zero_extend(datasize) << 12) : imm12.zero_extend(datasize);
            auto result = ArmUtils::add_with_carry(operand1, imm, false);
            state.set_gp(datasize, rd.as_i32(), result.first, true);

            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else if (op) {
            // sub
            bits operand1 = state.get_gp(datasize, n, true, false);
            bits imm = sh ? (imm12.zero_extend(datasize) << 12) : imm12.zero_extend(datasize);
            bits operand2 = ~imm;
            auto result = ArmUtils::add_with_carry(operand1, operand2, true);
            state.set_gp(datasize, d, result.first, true);

            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void InstructionExecution::visit_dp_imm_add_sub_with_tags(const Instruction &instruction) {
        // todo execute add sub with tag
    }

    void InstructionExecution::visit_dp_imm_logical(const Instruction &instruction) {
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

            bits operand1 = state.get_gp(datasize, n, false, true);
            megumin_assert(operand1.size == datasize);
            bits result = operand1 & imm;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b01) {
            // orr
            if (!sf && N) {
                megumin_assert(false);
            }

            bits operand1 = state.gp.get(datasize, n);
            megumin_assert(operand1.size == datasize);
            bits result = operand1 | imm;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b10) {
            // eor
            if (!sf && N) {
                megumin::megumin_assert(false);
            }

            bits operand1 = state.gp.get(datasize, n);
            megumin_assert(operand1.size == datasize);
            bits result = operand1 ^ imm;
            state.set_gp(datasize, d, result, false);
        } else if (opc == 0b11) {
            // ands
            if (!sf && N) {
                megumin_assert(false);
            }

            bits operand1 = state.gp.get(datasize, n);
            megumin_assert(operand1.size == datasize);
            bits result = operand1 & imm;
            state.set_gp(datasize, d, result, false);

            bits nzcv = bits::from_bools({result.is_set(datasize - 1), result == 0, 0, 0});
            state.p_state.set_nzcv(nzcv);
        }
    }

    void InstructionExecution::visit_dp_imm_move_wide(const Instruction &instruction) {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bits hw = instruction.get_range(21, 23);
        bits imm16 = instruction.get_range(5, 21);
        bits rd = instruction.get_range(0, 5);

        int datasize = sf ? 64 : 32;
        if (!sf && hw.is_set(1)) {
            megumin_assert(false);
        }

        bits pos_bits = hw.concat(bits{4, 0});
        int pos = pos_bits.as_u32();

        if (opc == 0b00) {
            // movn
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            result = ~result;
            state.gp.set(datasize, rd.as_i32(), result);
        } else if (opc == 0b10) {
            // movz
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            state.gp.set(datasize, rd.as_i32(), result);
        } else if (opc == 0b11) {
            // movk
            bits result = state.gp.get_ref(rd.as_i32());
            result.set_range(pos, pos + 16, imm16.as_i64());
            state.gp.set(datasize, rd.as_i32(), result);
        } else {
            megumin_assert(false);
        }
    }

    void InstructionExecution::visit_dp_imm_bitfield(const Instruction &instruction) {
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

        auto R = immr.as_u32();
        auto S = imms.as_u32();
        auto temp = ArmUtils::decode_bit_mask(datasize, N, imms, immr, false);
        wmask = temp.first;
        tmask = temp.second;

        if (opc == 0b00) {
            // sbfm
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = ArmUtilsSharedFunctions::ror(src, R) & wmask;
            bits top = ArmUtils::replicate(src.get_range(S, S + 1), datasize);
            bits result = (top & ~tmask) | (bot & tmask);
            state.gp.set(datasize, d, result);
        } else if (opc == 0b01) {
            // bfm
            bits dst = state.gp.get_ref(d).resize(datasize);
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = (dst & ~wmask) | (ArmUtilsSharedFunctions::ror(src, R) & wmask);
            bits result = (dst & ~tmask) | (bot & tmask);
            state.gp.set(datasize, d, result);
        } else if (opc == 0b10) {
            // ubfm
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = ArmUtilsSharedFunctions::ror(src, R) & wmask;
            state.gp.set(datasize, d, bot & tmask);
        }
    }

    void InstructionExecution::visit_dp_imm_extract(const Instruction &instruction) {
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

            int lsb = imms.as_u32();
            bits operand1 = state.gp.get_ref(n).resize(datasize);
            bits operand2 = state.gp.get_ref(m).resize(datasize);

            if (lsb == 0) {
                state.gp.set(datasize, d, operand2);
            } else {
                bits low = operand2.get_range(lsb, datasize);
                bits high = operand1.get_range(0, lsb);
                bits result = high.concat(low);
                state.gp.set(datasize, d, result);
            }
        } else {
            megumin::megumin_assert(false);
        }
    }

    void InstructionExecution::visit_dp_reg_1source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits opcode2 = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(10, 16);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        int n = rn.as_u32();
        int d = rd.as_u32();
        int datasize = sf ? 64 : 32;

        megumin_assert(S == 0);

        if (opcode2 == 0 && opcode == 0) {
            // rbit
            bits operand = state.gp.get_ref(n).resize(datasize);
            bits result {datasize, 0};
            for (int i = 0; i < datasize; i++) {
                bool b = operand.is_set(datasize - 1 - i);
                result.set_bit(i, b);
            }
            state.gp.set(datasize, d, result);
        } else if (opcode2 == 0 && (opcode >> 2) == 0) {
            // rev
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

            bits operand = state.gp.get(datasize, n);

            int containers = datasize / container_size;
            megumin_assert(containers >= 1);
            int elements_per_container = container_size / 8;
            int index = 0;
            int rev_index;

            bits result{datasize, 0};

            for (int c = 0; c < containers; c++) {
                rev_index = index + ((elements_per_container - 1) * 8);
                for (int e = 0; e < elements_per_container; e++) {
                    megumin_assert(rev_index <= datasize);
                    megumin_assert(rev_index + 8 <= datasize);
                    megumin_assert(index <= datasize);
                    megumin_assert(index + 8 <= datasize);
                    result.set_range(rev_index, rev_index + 8, operand.get_range(index, index + 8).as_i64());
                    index += 8;
                    rev_index -= 8;
                }
            }

            state.gp.set(datasize, d, result);
        } else if (opcode2 == 0 && opcode == 0b000100) {
            // clz
            bits operand1 = state.gp.get(datasize, n);
            int result = ArmUtilsSharedFunctions::count_leading_zero_bits(operand1);
            state.gp.set(datasize, d, bits{datasize, result});
        } else if (opcode2 == 0 && opcode == 0b000101) {
            // cls
            bits operand1 = state.gp.get(datasize, n);
            int result = ArmUtilsSharedFunctions::count_leading_sign_bits(operand1);
            state.gp.set(datasize, d, bits{datasize, result});
        }
        else {
            megumin_assert(false);
        }
    }

    void InstructionExecution::visit_dp_reg_2source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_range(16, 21);
        bits opc = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_u32();
        int n = rn.as_u32();
        int m = rm.as_u32();
        int datasize = sf ? 64 : 32;

        if (S == 0) {
            if (opc == 0b000010) {
                // udiv
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                if (operand2 == 0) {
                    state.gp.set(datasize, d, bits{datasize, 0});
                } else {
                    uint64_t op1 = operand1.as_u64();
                    uint64_t op2 = operand2.as_u64();
                    uint64_t result = op1 / op2;
                    state.gp.set(datasize, d, bits{datasize, static_cast<int64_t>(result)});
                }
            } else if (opc == 0b000011) {
                // sdiv
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                if (operand2 == 0) {
                    state.gp.set(datasize, d, bits{datasize, 0});
                } else {
                    if (datasize == 64) {
                        int64_t op1 = operand1.as_i64();
                        int64_t op2 = operand2.as_i64();

                        if (op1 == std::numeric_limits<int64_t>::min() && op2 == -1) {
                            // prevent overflow
                            state.gp.set(datasize, d, bits{datasize, 0});
                        } else {
                            int64_t result = op1 / op2;
                            state.gp.set(datasize, d, bits{datasize, result});
                        }
                    } else {
                        auto op1 = (int64_t) operand1.as_i32();
                        auto op2 = (int64_t) operand2.as_i32();

                        int64_t result = op1 / op2;
                        state.gp.set(datasize, d, bits{datasize, result});
                    }
                }
            } else if ((opc >> 2) == 0b10) {
                // lslv/lsrv/asrv/rorv
                int shift_type = instruction.get_range(10, 12).as_i32();
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                bits result = ArmUtils::shift_reg(operand1, shift_type, operand2.as_u32() % datasize);
                state.gp.set(datasize, d, result);
            } else {
                megumin_assert(false);
            }
        } else {
            megumin_assert(false);
        }
    }

    void InstructionExecution::visit_fp_simd_dp_1source(const Instruction &instruction) {
        bool M = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits ptype = instruction.get_range(22, 24);
        bits opcode = instruction.get_range(15, 21);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);
        int n = rn.as_i32();
        int d = rd.as_i32();

        int esize = 0;
        if (ptype == 0b00) {
            esize = 32;
        } else if (ptype == 0b01) {
            esize = 64;
        } else if (ptype == 0b10) {
            megumin_assert(false);
            esize = 64;
        } else if (ptype == 0b11) {
            esize = 16;
        }
        megumin_assert(esize != 0);
        megumin_assert(M == 0 && S == 0);

        if (opcode == 0b000000) {
            // fmov
            bits result = state.fp.get(esize, n);
            state.fp.set(esize, d, result);
        } else if (opcode == 0b000001) {
            // fabs

            bool merge = state.is_merging();

            bits operand = state.fp.get(esize, n);
            if (esize == 64) {
                double f = operand.as_f64();
                f = abs(f);
                bits result{f};
                state.fp.set(esize, d, result, merge);
            } else if (esize == 32) {
                float f = operand.as_f32();
                f = abs(f);
                bits result{f};
                state.fp.set(esize, d, result, merge);
            } else if (esize == 16) {
                // todo fp16
                megumin_assert(false);
            } else {
                megumin_assert(false);
            }
        } else if (opcode == 0b000010) {
            // fneg
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);
            // todo NaN
            bool sign = operand.is_set(esize - 1);
            operand.set_bit(esize - 1, !sign);
            state.fp.set(esize, d, operand, merge);
        } else if (opcode == 0b000011) {
            // fsqrt
            // 直接不采用比特操作了
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_sqrt(operand, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode[{2, 6}] == 0b0001) {
            // fcvt
            bits opc = opcode[{0, 2}];
            if (opc == ptype) {
                // undefined
                return;
            }

            int srcsize = esize;
            int dstsize = 0;
            if (opc == 0b00) {
                dstsize = 32;
            } else if (opc == 0b01) {
                dstsize = 64;
            } else if (opc == 0b10) {
                megumin::megumin_assert(false); // undefined
            } else {
                dstsize = 16;
            }

            if (srcsize == 16 || dstsize == 16) {
                megumin::megumin_assert("fp16 not supported");
            }

            bits operand = state.fp.get(srcsize, n);

            bool merge = state.is_merging();
            if (srcsize == 64) {
                double src = operand.as_f64();
                if (dstsize == 32) {
                    auto dst = static_cast<float>(src);
                    bits result{dst};
                    state.fp.set(dstsize, d, result, merge);
                } else {
                    megumin::megumin_assert(false);
                }
            } else if (srcsize == 32) {
                float src = operand.as_f32();
                if (dstsize == 64) {
                    auto dst = static_cast<double>(src);
                    bits result{dst};
                    state.fp.set(dstsize, d, result, merge);
                }
            } else {
                megumin::megumin_assert(false);
            }
        } else if (opcode == 0b001000) {
            // frintn
            arm::FPRounding rounding = FPUtils::fp_decode_rounding(0);
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);
            FPException exp;

            bits result = FPUtils::fp_round_int(operand, rounding, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001001) {
            // frintp
            arm::FPRounding rounding = FPUtils::fp_decode_rounding(1);
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, rounding, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001010) {
            // frintm
            arm::FPRounding rounding = FPUtils::fp_decode_rounding(2);
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, rounding, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001011) {
            // frintz
            arm::FPRounding rounding = FPUtils::fp_decode_rounding(3);
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, rounding, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001100) {
            // frinta
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, FPRounding::TIEAWAY, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001110) {
            // frintx
            arm::FPRounding rounding = state.get_rounding_mode();
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, rounding, true, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        } else if (opcode == 0b001111) {
            // frinti
            arm::FPRounding rounding = state.get_rounding_mode();
            bool merge = state.is_merging();
            bits operand = state.fp.get(esize, n);

            bits result = FPUtils::fp_round_int(operand, rounding, false, state.fp_exception);
            state.fp.set(esize, d, result, merge);
        }
    }

    void InstructionExecution::visit_fp_simd_dp_2source(const Instruction &instruction) {
        bool M = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits ptype = instruction.get_range(22, 24);
        bits rm = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(12, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int n = rn.as_i32();
        int d = rd.as_i32();
        int m = rm.as_i32();

        int esize = 0;
        if (ptype == 0b00) {
            esize = 32;
        } else if (ptype == 0b01) {
            esize = 64;
        } else if (ptype == 0b10) {
            megumin_assert(false);
            esize = 64;
        } else if (ptype == 0b11) {
            esize = 16;
        }
        megumin_assert(esize != 0);
        megumin_assert(M == 0 && S == 0);

        if (opcode == 0b0010) {
            // fadd
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            Float f1 = operand1.as_float();
            Float f2 = operand2.as_float();
            Float result = f1 + f2;
            state.fp.set(esize, d, result.to_bits());
        } else if (opcode == 0b0011) {
            // fsub
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            Float f1 = operand1.as_float();
            Float f2 = operand2.as_float();
            Float result = f1 - f2;
            state.fp.set(esize, d, result.to_bits());
        } else if (opcode == 0b0100) {
            // fmax
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            bits result = FPUtils::fp_max(operand1, operand2, false, state.fp_exception);
            state.fp.set(esize, d, result);
        } else if (opcode == 0b0101) {
            // fmin
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            bits result = FPUtils::fp_min(operand1, operand2, false, state.fp_exception);
            state.fp.set(esize, d, result);
        } else if (opcode == 0b0000) {
            // fmul
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            if (esize == 64) {
                double op1 = operand1.as_f64();
                double op2 = operand2.as_f64();
                double result = op1 * op2;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float op1 = operand1.as_f32();
                float op2 = operand2.as_f32();
                float result = op1 * op2;
                state.fp.set(esize, d, bits{result});
            } else {
                megumin::megumin_assert(false);
            }
        } else if (opcode == 0b0001) {
            // fdiv
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            if (esize == 64) {
                double op1 = operand1.as_f64();
                double op2 = operand2.as_f64();
                double result = op1 / op2;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float op1 = operand1.as_f32();
                float op2 = operand2.as_f32();
                float result = op1 / op2;
                state.fp.set(esize, d, bits{result});
            } else {
                megumin::megumin_assert(false);
            }
        } else if (opcode == 0b0110) {
            // fmaxnm
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            if (esize == 64) {
                double op1 = operand1.as_f64();
                double op2 = operand2.as_f64();
                double result = fmax(op1, op2);
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float op1 = operand1.as_f32();
                float op2 = operand2.as_f32();
                float result = fmax(op1, op2);
                state.fp.set(esize, d, bits{result});
            } else {
                megumin::megumin_assert(false);
            }
        } else if (opcode == 0b0111) {
            // fminnm
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            if (esize == 64) {
                double op1 = operand1.as_f64();
                double op2 = operand2.as_f64();
                double result = fmin(op1, op2);
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float op1 = operand1.as_f32();
                float op2 = operand2.as_f32();
                float result = fmin(op1, op2);
                state.fp.set(esize, d, bits{result});
            } else {
                megumin::megumin_assert(false);
            }
        } else if (opcode == 0b1000) {
            // fnmul
            bits operand1 = state.fp.get(esize, n);
            bits operand2 = state.fp.get(esize, m);
            if (esize == 64) {
                double op1 = operand1.as_f64();
                double op2 = operand2.as_f64();
                double result = -op1 * op2;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float op1 = operand1.as_f32();
                float op2 = operand2.as_f32();
                float result = -op1 * op2;
                state.fp.set(esize, d, bits{result});
            } else {
                megumin::megumin_assert(false);
            }
        } else {
            megumin::megumin_assert(false);
        }
    }

    void InstructionExecution::visit_dp_reg_logical_shifted_reg(const Instruction &instruction) {
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

        bits operand1 = state.gp.get(datasize, n);
        bits operand2 = ArmUtils::shift_reg(state.gp.get(datasize, m), shift.as_i32(), shift_amount);
        if (N) {
            operand2 = ~operand2;
        }

        if (opc == 0b00) {
            // and bic
            bits result = operand1 & operand2;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b01) {
            // orr orn
            bits result = operand1 | operand2;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b10) {
            // eor eon
            bits result = operand1 ^ operand2;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b11) {
            // ands bics
            bits result = operand1 & operand2;
            bits nzcv = bits::from_bools({result.is_set(datasize - 1), result == 0, 0, 0});
            state.gp.set(datasize, d, result);
            state.p_state.set_nzcv(nzcv);
        }
    }

    void InstructionExecution::visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) {
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

        bits operand1 = state.gp.get(datasize, n);
        bits operand2 = ArmUtils::shift_reg(state.gp.get(datasize, m), shift.as_i32(), shift_amount);

        if (op == 0) {
            // add adds
            auto result = ArmUtils::add_with_carry(operand1, operand2, false);
            state.gp.set(datasize, d, result.first);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else if (op == 1) {
            // sub subs
            operand2 = ~operand2;
            auto result = ArmUtils::add_with_carry(operand1, operand2, true);
            state.gp.set(datasize, d, result.first);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void InstructionExecution::visit_dp_reg_add_sub_with_carry(const Instruction &instruction) {
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

        bits operand1 = state.gp.get(datasize, n);
        bits operand2 = state.gp.get(datasize, m);

        if (op == 0) {
            auto result = ArmUtils::add_with_carry(operand1, operand2, state.p_state.get_c());
            state.gp.set(datasize, d, result.first);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        } else if (op == 1) {
            operand2 = ~operand2;
            auto result = ArmUtils::add_with_carry(operand1, operand2, state.p_state.get_c());
            state.gp.set(datasize, d, result.first);
            if (S) {
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void InstructionExecution::visit_dp_reg_cond_select(const Instruction &instruction) {
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
            if (ArmUtilsSharedFunctions::condition_holds(cond.as_i32(), state)) {
                bits result = state.gp.get(datasize, n);
                state.gp.set(datasize, d, result);
            } else {
                bits result = state.gp.get(datasize, m);
                state.gp.set(datasize, d, result);
            }
        } else if (op == 0 && op2 == 0b01) {
            // csinc
            if (ArmUtilsSharedFunctions::condition_holds(cond.as_i32(), state)) {
                bits result = state.gp.get(datasize, n);
                state.gp.set(datasize, d, result);
            } else {
                bits result = state.gp.get(datasize, m);
                result = result + 1;
                state.gp.set(datasize, d, result);
            }
        } else if (op == 1 && op2 == 0b00) {
            // csinv
            if (ArmUtilsSharedFunctions::condition_holds(cond.as_i32(), state)) {
                bits result = state.gp.get(datasize, n);
                state.gp.set(datasize, d, result);
            } else {
                bits result = state.gp.get(datasize, m);
                result = ~result;
                state.gp.set(datasize, d, result);
            }
        } else if (op == 1 && op2 == 0b01) {
            // csneg
            if (ArmUtilsSharedFunctions::condition_holds(cond.as_i32(), state)) {
                bits result = state.gp.get(datasize, n);
                state.gp.set(datasize, d, result);
            } else {
                bits result = state.gp.get(datasize, m);
                result = ~result + 1;
                state.gp.set(datasize, d, result);
            }
        }
    }

    void InstructionExecution::visit_dp_reg_3source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits op54 = instruction.get_range(29, 31);
        bits op31 = instruction.get_range(21, 24);
        bits rm = instruction.get_rm();
        bool o0 = instruction.is_set(15);
        bits ra = instruction.get_range(10, 15);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();
        bits op = op31.concat(bits::from_bools({o0}));

        megumin::megumin_assert(op54 == 0);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        int a = ra.as_i32();
        int datasize = sf ? 64 : 32;

        bits operand1 = state.get_gp(datasize, n, false, true);
        bits operand2 = state.get_gp(datasize, m, false, true);
        bits operand3 = state.get_gp(datasize, a, false, true);

        if (op == 0b0000) {
            // madd
            if (datasize == 32) {
                uint32_t op1 = operand1.as_u32();
                uint32_t op2 = operand2.as_u32();
                uint32_t op3 = operand3.as_u32();
                uint32_t result = op3 + op1 * op2;
                state.set_gp(datasize, d, bits::from_u32(result), false);
            } else if (datasize == 64) {
                uint64_t result = operand3.as_u64() + operand1.as_u64() * operand2.as_u64();
                state.set_gp(datasize, d, bits::from_u64(result), false);
            }
        } else if (op == 0b0001) {
            // msub
            if (datasize == 32) {
                uint32_t result = operand3.as_u32() - operand1.as_u32() * operand2.as_u32();
                state.set_gp(32, d, bits::from_u32(result), false);
            } else if (datasize == 64) {
                uint64_t result = operand3.as_u64() - operand1.as_u64() * operand2.as_u64();
                state.set_gp(datasize, d, bits::from_u64(result), false);
            }
        } else if (op == 0b0010) {
            // smaddl
            megumin::megumin_assert(datasize == 64);

            int32_t op1 = state.get_gp(32, n, false, true).as_i32();
            int32_t op2 = state.get_gp(32, m, false, true).as_i32();
            int64_t op3 = state.get_gp(64, a, false, true).as_i64();
            uint64_t temp1 = static_cast<uint64_t>(op1);
            uint64_t temp2 = static_cast<uint64_t>(op2);
            int64_t temp3 = static_cast<int64_t>(temp1);
            int64_t temp4 = static_cast<int64_t>(temp2);
            int64_t result = op3 + temp3 * temp4;
            state.set_gp(64, d, bits{64, result}, false);
        } else if (op == 0b0011) {
            // smsubl
            megumin::megumin_assert(datasize == 64);

            int32_t op1 = state.get_gp(32, n, false, true).as_i32();
            int32_t op2 = state.get_gp(32, m, false, true).as_i32();
            int64_t op3 = state.get_gp(64, a, false, true).as_i64();
            uint64_t temp1 = static_cast<uint64_t>(op1);
            uint64_t temp2 = static_cast<uint64_t>(op2);
            int64_t temp3 = static_cast<int64_t>(temp1);
            int64_t temp4 = static_cast<int64_t>(temp2);
            int64_t result = op3 - temp3 * temp4;
            state.set_gp(64, d, bits{64, result}, false);
        } else if (op == 0b0100) {
            // smulh
            megumin::megumin_assert(datasize == 64);

            int64_t op1 = operand1.as_i64();
            int64_t op2 = operand2.as_i64();

            absl::int128 temp = absl::MakeInt128(0, op1) * absl::MakeInt128(0, op2);
            int64_t high = absl::Int128High64(temp);
            state.set_gp(64, d, bits{64, high}, false);
        } else if (op == 0b1010) {
            // umaddl
            megumin::megumin_assert(datasize == 64);

            uint32_t op1 = state.get_gp(32, n, false, true).as_i32();
            uint32_t op2 = state.get_gp(32, m, false, true).as_i32();
            uint64_t op3 = state.get_gp(64, a, false, true).as_i64();
            uint64_t temp1 = static_cast<uint64_t>(op1);
            uint64_t temp2 = static_cast<uint64_t>(op2);
            uint64_t result = op3 + temp1 * temp2;
            state.set_gp(64, d, bits{64, (int64_t) result}, false);
        } else if (op == 0b1011) {
            // umsubl
            megumin::megumin_assert(datasize == 64);

            uint32_t op1 = state.get_gp(32, n, false, true).as_i32();
            uint32_t op2 = state.get_gp(32, m, false, true).as_i32();
            uint64_t op3 = state.get_gp(64, a, false, true).as_i64();
            uint64_t temp1 = static_cast<uint64_t>(op1);
            uint64_t temp2 = static_cast<uint64_t>(op2);
            uint64_t result = op3 - temp1 * temp2;
            state.set_gp(64, d, bits{64, (int64_t) result}, false);
        } else if (op == 0b1100) {
            // umulh
            megumin::megumin_assert(datasize == 64);

            uint64_t op1 = operand1.as_u64();
            uint64_t op2 = operand2.as_u64();

            absl::uint128 temp = absl::MakeUint128(0, op1) * absl::MakeUint128(0, op2);
            int64_t high = (int64_t) absl::Uint128High64(temp);
            state.set_gp(64, d, bits{64, high}, false);
        } else {
            megumin::megumin_assert(false);
        }
    }

    void InstructionExecution::visit_fp_simd_imm(const Instruction &instruction) {
        bits ptype = instruction.get_range(22, 24);
        bits imm8 = instruction.get_range(13, 21);
        bits imm5 = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();

        megumin::megumin_assert(!instruction.is_set(31));
        megumin::megumin_assert(!instruction.is_set(29));

        int size = 0;
        if (ptype == 0b00) {
            size = 32;
        } else if (ptype == 0b01) {
            size = 64;
        } else {
            megumin::megumin_assert(false);
        }

        bits imm = FPUtils::vfp_expand_imm(size, imm8);
        state.fp.set(size, d, imm, false);
    }

    void InstructionExecution::visit_fp_simd_dp_3source(const Instruction &instruction) {
        bits ptype = instruction.get_range(22, 24);
        bool o1 = instruction.is_set(21);
        bits rm = instruction.get_range(16, 21);
        bool o0 = instruction.is_set(15);
        bits ra = instruction.get_range(10, 15);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int a = ra.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        bool merging = state.is_merging();

        int esize = 0;
        if (ptype == 0b00) {
            esize = 32;
        } else if (ptype == 0b01) {
            esize = 64;
        } else {
            megumin::megumin_assert(false);
        }

        bits operanda = state.fp.get(esize, a);
        bits operand1 = state.fp.get(esize, n);
        bits operand2 = state.fp.get(esize, m);

        bits op = bits::from_bools({o1, o0});
        if (op == 0b00) {
            // fmadd
            if (esize == 64) {
                double v1 = operand1.as_f64();
                double v2 = operand2.as_f64();
                double va = operanda.as_f64();
                double result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float v1 = operand1.as_f32();
                float v2 = operand2.as_f32();
                float va = operanda.as_f32();
                float result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            }
        } else if (op == 0b01) {
            // fmsub
            if (esize == 64) {
                double v1 = -operand1.as_f64();
                double v2 = operand2.as_f64();
                double va = operanda.as_f64();
                double result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float v1 = -operand1.as_f32();
                float v2 = operand2.as_f32();
                float va = operanda.as_f32();
                float result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            }
        } else if (op == 0b10) {
            // fnmadd
            if (esize == 64) {
                double v1 = -operand1.as_f64();
                double v2 = operand2.as_f64();
                double va = -operanda.as_f64();
                double result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float v1 = -operand1.as_f32();
                float v2 = operand2.as_f32();
                float va = -operanda.as_f32();
                float result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            }
        } else if (op == 0b11) {
            // fnmsub
            if (esize == 64) {
                double v1 = operand1.as_f64();
                double v2 = operand2.as_f64();
                double va = -operanda.as_f64();
                double result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            } else if (esize == 32) {
                float v1 = operand1.as_f32();
                float v2 = operand2.as_f32();
                float va = -operanda.as_f32();
                float result = v1 * v2 + va;
                state.fp.set(esize, d, bits{result});
            }
        }
    }

    void InstructionExecution::visit_fp_compare(const Instruction &instruction) {
        bits ptype = instruction.get_range(22, 24);
        bits rm = instruction.get_rm();
        bits rn = instruction.get_rn();
        bits opcode2 = instruction.get_range(0, 5);

        int m = rm.as_i32();
        int n = rn.as_i32();
        int datasize = 0;
        if (ptype == 0b00) {
            datasize = 32;
        } else if (ptype == 0b01) {
            datasize = 64;
        } else {
            megumin::megumin_assert(false);
            datasize = 64;
        }
        bool signal_all_nans = opcode2[4] == 1;
        bool cmp_with_zero = opcode2[3] == 1;
        bits operand1 = state.fp.get(datasize, n);
        bits operand2;
        if (cmp_with_zero) {
            operand2 = bits::fpzero(false, datasize);
        } else {
            operand2 = state.fp.get(datasize, m);
        }

        bits result = FPUtils::fp_compare(operand1, operand2, signal_all_nans, state.fp_exception);
        state.p_state.set_nzcv(result);
    }
}
