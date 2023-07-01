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
}
