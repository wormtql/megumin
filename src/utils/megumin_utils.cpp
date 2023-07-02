//
// Created by 58413 on 2023/6/2.
//

#include "megumin_utils.h"
#include <iostream>

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
}
