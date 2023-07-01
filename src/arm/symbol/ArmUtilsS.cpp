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
        expr nsum = nx + ny;

        expr sign_x = x.extract(size - 1, size - 1);
        expr sign_y = y.extract(size - 1, size - 1);
        expr sign_sum = sum.extract(size - 1, size - 1);

        expr n = sum.extract(size - 1, size - 1);
        expr z = sum == 0;
        expr c = nsum.extract(size + 1, size + 1);
        expr v = (sign_x == sign_y && sign_x != sign_sum);

        PStateS p_state{n, z, c, v};
        return { sum, p_state };
    }

    std::pair<expr, PStateS> ArmUtilsS::add_with_carry(const expr& x, bits y, bool carry) {
        megumin::megumin_assert(x.is_bv());
        megumin::megumin_assert(x.get_sort().bv_size() == y.size);
        megumin::megumin_assert(y.data0 < (1 << 12));


        auto size = x.get_sort().bv_size();
//        auto t1 = x + (int) carry;
//        auto t2 = t1 + x.ctx().num_val(y.as_i32(), x.ctx().bv_sort(size));
        auto sum = x + x.ctx().num_val(y.as_i32() + (int) carry, x.ctx().bv_sort(size));
//        auto sum = t2;

        expr nx = z3::zext(x, 1);
//        expr nsum = nx + x.ctx().int_val(y.as_u64()) + (int) carry;
        expr nsum = nx + x.ctx().num_val(y.as_i32() + (int) carry, x.ctx().bv_sort(size + 1));

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
        expr result = z3::lshr(x, shift);

        return {result, carry_out};
    }
}
