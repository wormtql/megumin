//
// Created by 58413 on 2023/6/2.
//

#include "megumin_utils.h"
#include <iostream>
#include "symbol/ArmUtilsS.h"

inline int64_t get_mask(int size) {
    if (size == 64) {
        return ~0ll;
    } else {
        return (1ll << size) - 1;
    }
}

namespace megumin {
    void megumin_assert(bool expression, std::optional<std::string> message) {
        if (!expression) {
            if (message.has_value()) {
                std::cerr << "assertion failed: " << message.value() << std::endl;
            }
        }
    }

    [[noreturn]] void megumin_todo() {
        megumin_assert(false, "todo");
    }

    [[noreturn]] void megumin_unreachable() {
        megumin_assert(false, "unreachable");
    }

    string trim(const string& s) {
        const char* WhiteSpace = " \t\v\r\n";
        std::size_t start = s.find_first_not_of(WhiteSpace);
        std::size_t end = s.find_last_not_of(WhiteSpace);
        return start == end ? std::string() : s.substr(start, end - start + 1);
    }

    expr set_expr_range(expr x, int low, int high, int64_t value) {
        megumin_assert(x.is_bv());

        int64_t mask = get_mask(high) - get_mask(low);
        int64_t maskr = ~mask;

        auto& c = x.ctx();
        auto result = x;
        int size = x.get_sort().bv_size();
        result = result & c.bv_val(maskr, size);

        value <<= low;
        value &= mask;

        result = result | c.bv_val(value, size);

        return result;
    }

    expr set_expr_range(expr x, int low, int high, expr value) {
        megumin_assert(x.is_bv());

        int64_t mask = get_mask(high) - get_mask(low);
        int64_t maskr = ~mask;

        auto& c = x.ctx();
        auto result = x;
        int size = x.get_sort().bv_size();
        result = result & c.bv_val(maskr, size);

        value = z3::zext(value, size - value.get_sort().bv_size());
        value = z3::shl(value, low);
        value = value & c.bv_val(mask, size);

        result = result | value;

        return result;
    }

    // https://yurichev.com/blog/bitrev/
    expr reverse_expr(const expr& x) {
        megumin_assert(x.get_sort().is_bv());
        int size = x.get_sort().bv_size();

        expr a = x;
        auto& c = x.ctx();

        if (size == 64) {
            a = z3::lshr(a, 32) ^ z3::shl(a, 32);
            auto m = c.bv_val(0x0000ffff0000ffff, 64);
            a = (z3::lshr(a, 16) & m) ^ ((z3::shl(a, 16) & ~m));
            m = c.bv_val(0x00ff00ff00ff00ff, 64);
            a = (z3::lshr(a, 8) & m) ^ ((z3::shl(a, 8) & ~m));
            m = c.bv_val(0x0f0f0f0f0f0f0f0f, 64);
            a = (z3::lshr(a, 4) & m) ^ (z3::shl(a, 4) & ~m);
            m = c.bv_val(0x3333333333333333, 64);
            a = (z3::lshr(a, 2) & m) ^ (z3::shl(a, 2) & ~m);
            m = c.bv_val(0x5555555555555555, 64);
            a = (z3::lshr(a, 1) & m) ^ (z3::shl(a, 1) & ~m);
            return a;
        } else if (size == 32) {
            auto bv = [&] (uint64_t v) {
                return c.bv_val(v, 32);
            };
            a = z3::lshr(a, 16) | z3::shl(a, 16);
            a = z3::lshr(a & bv(0xFF00FF00), 8) | z3::shl(a & bv(0x00FF00FF), 8);
            a = z3::lshr(a & bv(0xF0F0F0F0), 4) | z3::shl(a & bv(0x0F0F0F0F), 4);
            a = z3::lshr(a & bv(0xCCCCCCCC), 2) | z3::shl(a & bv(0x33333333), 2);
            a = z3::lshr(a & bv(0xAAAAAAAA), 1) | z3::shl(x & bv(0x55555555), 1);
            return a;
        } else {
            megumin_assert(false);
        }
    }

    expr z3_bool_to_bv(const expr& b, int size) {
        auto& c = b.ctx();

        expr zero = c.bv_val(0, size);
        expr one = c.bv_val(1, size);

        return z3::ite(b, one, zero);
    }

    expr round_up_to_next_power2_64(const expr& x) {
        auto o1 = x - 1;
        auto o2 = z3::lshr(o1, 1);
        auto o3 = o2 | o1;
        auto o4 = z3::lshr(o3, 2);
        auto o5 = o3 | o4;
        auto o6 = z3::lshr(o5, 4);
        auto o7 = o5 | o6;
        auto o8 = z3::lshr(o7, 8);
        auto o9 = o7 | o8;
        auto o10 = z3::lshr(o9, 16);
        auto o11 = o9 | o10;
        auto o12 = z3::lshr(o11, 32);
        auto o13 = o11 | o12;
        return o13 + 1;
    }

    expr round_up_to_next_power2_32(const expr& x) {
        auto o1 = x - 1;
        auto o2 = z3::lshr(o1, 1);
        auto o3 = o2 | o1;
        auto o4 = z3::lshr(o3, 2);
        auto o5 = o3 | o4;
        auto o6 = z3::lshr(o5, 4);
        auto o7 = o5 | o6;
        auto o8 = z3::lshr(o7, 8);
        auto o9 = o7 | o8;
        auto o10 = z3::lshr(o9, 16);
        auto o11 = o9 | o10;
        return o11 + 1;
    }

    expr count_leading_zero_64(const expr& x, int size) {
        expr low = x.extract(31, 0);
        expr high = x.extract(63, 32);

        return z3::ite(high == 0, 32 + count_leading_zero_32(low, size), count_leading_zero_32(high, size));
    }

    expr count_leading_zero_32(const expr& x, int size) {
        expr low = x.extract(15, 0);
        expr high = x.extract(31, 16);

        return z3::ite(high == 0, 16 + count_leading_zero_16(low, size), count_leading_zero_16(high, size));
    }

    expr count_leading_zero_16(const expr& x, int size) {
        expr low = x.extract(7, 0);
        expr high = x.extract(15, 8);

        return z3::ite(high == 0, 8 + count_leading_zero_8(low, size), count_leading_zero_8(high, size));
    }

    expr count_leading_zero_8(const expr& x, int size) {
        expr low = x.extract(3, 0);
        expr high = x.extract(7, 4);

        return z3::ite(high == 0, 4 + count_leading_zero_4(low, size), count_leading_zero_4(high, size));
    }

    expr count_leading_zero_4(const expr& x, int size) {
        expr low = x.extract(1, 0);
        expr high = x.extract(3, 2);

        return z3::ite(high == 0, 2 + count_leading_zero_2(low, size), count_leading_zero_2(high, size));
    }

    expr count_leading_zero_2(const expr& x, int size) {
        expr low = x.extract(0, 0);
        expr high = x.extract(1, 1);
        auto& c = x.ctx();

        return z3::ite(
                high == 0,
                z3::ite(
                        low == 0,
                        c.bv_val(2, size),
                        c.bv_val(1, size)
                ),
                c.bv_val(0, size)
        );
    }

    expr count_leading_zero(int size, const expr& x) {
        if (size == 64) {
            return count_leading_zero_64(x, size);
        } else if (size == 32) {
            return count_leading_zero_32(x, size);
        } else {
            megumin_assert(false);
        }
    }

    expr count_leading_signed(const expr& x, int size) {
        auto& c = x.ctx();
        int bv_size = x.get_sort().bv_size();
        int s = bv_size / 2;

        if (bv_size == 1) {
            return z3::ite(x == 1, c.bv_val(1, size), c.bv_val(0, size));
        }

        expr low = x.extract(s - 1, 0);
        expr high = x.extract(s * 2 - 1, s);


        return z3::ite(
                high == c.bv_val((uint64_t)(((uint64_t) 1 << s) - 1), s),
                s + count_leading_signed(low, size),
                count_leading_signed(high, size)
        );
    }
}
