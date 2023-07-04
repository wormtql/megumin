//
// Created by 58413 on 2023/6/27.
//

#include "ArmUtilsS.h"
#include "megumin_utils.h"
#include "ArmUtils.h"

namespace arm {
    std::pair<expr, PStateS> ArmUtilsS::add_with_carry(expr x, expr y, expr carry) {
        megumin::megumin_assert(x.is_bv() && y.is_bv());
        megumin::megumin_assert(x.get_sort().bv_size() == y.get_sort().bv_size());

        auto size = x.get_sort().bv_size();
        auto sum = x + y + carry;

        expr nx = z3::zext(x, 1);
        expr ny = z3::zext(y, 1);
        expr ncarry = z3::zext(carry, 1);
        expr nsum = nx + ny + ncarry;

        expr sign_x = x.extract(size - 1, size - 1);
        expr sign_y = y.extract(size - 1, size - 1);
        expr sign_sum = sum.extract(size - 1, size - 1);

        expr n = sum.extract(size - 1, size - 1) == 1;
        expr z = sum == 0;
        expr c = nsum.extract(size, size) == 1;
        expr v = (sign_x == sign_y && sign_x != sign_sum);

        megumin::megumin_assert(n.is_bool());
        megumin::megumin_assert(z.is_bool());
        megumin::megumin_assert(c.is_bool());
        megumin::megumin_assert(v.is_bool());

        PStateS p_state{n, z, c, v};
        return { sum, p_state };
    }

    std::pair<expr, PStateS> ArmUtilsS::add_with_carry(const expr& x, bits y, bool carry) {
        megumin::megumin_assert(x.is_bv());
        megumin::megumin_assert(x.get_sort().bv_size() == y.size);
//        megumin::megumin_assert(y.data0 < (1 << 12));

        auto& ctx = x.ctx();

        auto size = x.get_sort().bv_size();
//        auto t1 = x + (int) carry;
//        auto t2 = t1 + x.ctx().num_val(y.as_i32(), x.ctx().bv_sort(size));
        expr yy = ctx.bv_val(y.data0, size);
        auto sum = x + yy + ctx.bv_val((int) carry, size);
//        auto sum = t2;

        expr nx = z3::zext(x, 1);
        expr nyy = z3::zext(yy, 1);
        expr nsum = nx + nyy + ctx.bv_val((int) carry, size + 1);

        expr sign_x = x.extract(size - 1, size - 1);
        bool sign_y = y.is_set(size - 1);
        expr sign_sum = sum.extract(size - 1, size - 1);

        expr n = sum.extract(size - 1, size - 1) == 1;
        expr z = sum == 0;
        expr c = nsum.extract(size, size) == 1;
        expr v = (sign_x == sign_y && sign_x != sign_sum);

        megumin::megumin_assert(n.is_bool());
        megumin::megumin_assert(z.is_bool());
        megumin::megumin_assert(c.is_bool());
        megumin::megumin_assert(v.is_bool());

        PStateS p_state{n, z, c, v};
        return { sum, p_state };
    }

    expr ArmUtilsS::replicate(const expr &x, int m) {
        megumin::megumin_assert(x.get_sort().is_bv());
        int size = x.get_sort().bv_size();
        megumin::megumin_assert(size * m <= 64);

        return x.repeat(m);
    }

    expr ArmUtilsS::shift_reg(const expr& reg, int shift_type, int amount) {
        switch (shift_type) {
            case 0:
                return ArmUtilSharedFunctionsS::lsl(reg, amount);
            case 1:
                return ArmUtilSharedFunctionsS::lsr(reg, amount);
            case 2:
                return ArmUtilSharedFunctionsS::asr(reg, amount);
            case 3:
                return ArmUtilSharedFunctionsS::ror(reg, amount);
        }
        megumin::megumin_unreachable();
    }

    expr ArmUtilsS::shift_reg(const expr &reg, int shift_type, const expr &amount) {
        switch (shift_type) {
            case 0:
                return ArmUtilSharedFunctionsS::lsl(reg, amount);
            case 1:
                return ArmUtilSharedFunctionsS::lsr(reg, amount);
            case 2:
                return ArmUtilSharedFunctionsS::asr(reg, amount);
            case 3:
                return ArmUtilSharedFunctionsS::ror(reg, amount);
        }
        megumin::megumin_unreachable();
    }

    std::pair<expr, PStateS> ArmUtilsS::add_with_carry(const expr &x, const expr &y, bool carry) {
        auto& c = x.ctx();
        int size = x.get_sort().bv_size();

        if (carry) {
            return add_with_carry(x, y, c.bv_val(1, size));
        } else {
            return add_with_carry(x, y, c.bv_val(0, size));
        }
    }
}

namespace arm {
    expr ArmUtilSharedFunctionsS::lsr(const expr& x, int shift) {
        megumin::megumin_assert(shift >= 0);
        if (shift == 0) {
            return x;
        } else {
            auto result = lsr_c(x, shift);
            return result.first;
        }
    }

    std::pair<expr, expr> ArmUtilSharedFunctionsS::lsr_c(const expr &x, int shift) {
        megumin::megumin_assert(shift > 0);
        megumin::megumin_assert(x.get_sort().is_bv());

//        int N = x.get_sort().bv_size();

        expr carry_out = x.extract(shift - 1, shift - 1);

        expr result = z3::lshr(x, shift);

        return {result, carry_out};
    }

    expr ArmUtilSharedFunctionsS::lsl(const expr &x, int shift) {
        megumin::megumin_assert(shift >= 0);
        megumin::megumin_assert(x.get_sort().is_bv());

        if (shift == 0) {
            return x;
        } else {
            auto result = lsl_c(x, shift);
            return result.first;
        }
    }

    std::pair<expr, expr> ArmUtilSharedFunctionsS::lsl_c(const expr &x, int shift) {
        megumin::megumin_assert(shift > 0);
        megumin::megumin_assert(x.get_sort().is_bv());

        int N = x.get_sort().bv_size();

        expr result = z3::shl(x, shift);
        expr carry_out = x.extract(N - shift, N - shift);

        return {result, carry_out};
    }

    expr ArmUtilSharedFunctionsS::ror(const expr& x, int shift) {
        megumin::megumin_assert(shift >= 0);
        megumin::megumin_assert(x.get_sort().is_bv());
        if (shift == 0) {
            return x;
        } else {
            auto result = ror_c(x, shift);
            return result.first;
        }
    }

    std::pair<expr, expr> ArmUtilSharedFunctionsS::ror_c(const expr& x, int shift) {
        megumin::megumin_assert(shift != 0);
        megumin::megumin_assert(x.get_sort().is_bv());

        int N = x.get_sort().bv_size();
        int m = shift % N;
        auto result = lsr(x, m) | lsl(x, N - m);
        expr carry_out = result.extract(N - 1, N - 1);
        return {result, carry_out};
    }

    expr ArmUtilSharedFunctionsS::asr(const expr& x, int shift) {
        megumin::megumin_assert(shift >= 0);
        megumin::megumin_assert(x.get_sort().is_bv());

        if (shift == 0) {
            return x;
        } else {
            return asr_c(x, shift).first;
        }
    }

    std::pair<expr, expr> ArmUtilSharedFunctionsS::asr_c(const expr &x, int shift) {
        megumin::megumin_assert(shift > 0);
        megumin::megumin_assert(x.get_sort().is_bv());
        int size = x.get_sort().bv_size();
        megumin::megumin_assert(shift < size);

        expr carry_out = x.extract(shift - 1, shift - 1);
        expr result = z3::ashr(x, shift);

        return {result, carry_out};
    }

    expr ArmUtilSharedFunctionsS::ror(const expr &x, const expr &shift) {
        megumin::megumin_assert(x.get_sort().is_bv());
        megumin::megumin_assert(shift.get_sort().is_bv());

        int size = x.get_sort().bv_size();

        auto result = ror_c(x, shift);
        return result.first;
    }

    std::pair<expr, expr> ArmUtilSharedFunctionsS::ror_c(const expr &x, const expr &shift) {
        megumin::megumin_assert(x.get_sort().is_bv());
        megumin::megumin_assert(shift.get_sort().is_bv());

        int N = x.get_sort().bv_size();
        expr m = shift % N;
        expr result = lsr(x, m) | lsl(x, N - m);
        expr carry_out = result.extract(N - 1, N - 1);
        return {result, carry_out};
    }

    expr ArmUtilSharedFunctionsS::lsr(const expr &x, const expr& shift) {
        megumin::megumin_assert(x.is_bv());

        int size = x.get_sort().bv_size();
        return z3::lshr(x, shift % size);
    }

//    std::pair<expr, expr> ArmUtilSharedFunctionsS::lsr_c(const expr &x, const expr &shift) {
//        megumin::megumin_assert(x.get_sort().is_bv());
//
//        auto& c = x.ctx();
//        expr carry_out = x.extract(shift - 1, c.bv_val(1, x.get_sort().bv_size()));
//        expr result = z3::lshr(x, shift);
//
//        return {result, carry_out};
//    }

    expr ArmUtilSharedFunctionsS::lsl(const expr &x, const expr &shift) {
        megumin::megumin_assert(x.is_bv());

        int size = x.get_sort().bv_size();
        return z3::shl(x, shift % size);
    }

//    std::pair<expr, expr> ArmUtilSharedFunctionsS::lsl_c(const expr &x, const expr &shift) {
//        megumin::megumin_assert(x.get_sort().is_bv());
//
//        auto& c = x.ctx();
//        int N = x.get_sort().bv_size();
//
//        expr result = z3::shl(x, shift);
//        expr temp = N - shift;
//        auto t = Z3_mk_seq_extract(c, x, temp, c.bv_val(1, N));
//        auto s = Z3_get_error_msg(c, Z3_EXCEPTION);
//        // check_error(); return expr(ctx(), r);
//
//
//        expr carry_out = x.extract(temp, c.bv_val(1, N));
//
//        return {result, carry_out};
//    }

    expr ArmUtilSharedFunctionsS::asr(const expr &x, const expr &shift) {
        megumin::megumin_assert(x.is_bv());

        int size = x.get_sort().bv_size();
        return z3::ashr(x, shift % size);
    }

    expr ArmUtilSharedFunctionsS::condition_holds(int cond, const MachineStateS &state) {
        int temp = cond >> 1;
        auto& ctx = state.sp.ctx();
        expr result = ctx.bool_val(false);

        switch (temp) {
            case 0b000:
                result = state.p_state.z;
                break;
            case 0b001:
                result = state.p_state.c;
                break;
            case 0b010:
                result = state.p_state.n;
                break;
            case 0b011:
                result = state.p_state.v;
                break;
            case 0b100:
                result = state.p_state.c && !state.p_state.z;
                break;
            case 0b101:
                result = state.p_state.n == state.p_state.v;
                break;
            case 0b110:
                result = state.p_state.n == state.p_state.v && !state.p_state.z;
                break;
            case 0b111:
                result = ctx.bool_val(true);
                break;
        }

        if (cond % 2 == 1 && cond != 0b1111) {
            result = !result;
        }

        return result;
    }

//    std::pair<expr, expr> ArmUtilSharedFunctionsS::asr_c(const expr &x, const expr &shift) {
//        megumin::megumin_assert(x.get_sort().is_bv());
//
//        auto& c = x.ctx();
//
//        expr carry_out = x.extract(shift - 1, c.bv_val(1, x.get_sort().bv_size()));
//        expr result = z3::ashr(x, shift);
//
//        return {result, carry_out};
//    }
}
