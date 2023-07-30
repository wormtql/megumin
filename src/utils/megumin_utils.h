//
// Created by 58413 on 2023/6/2.
//

#ifndef MEGUMIN_MEGUMIN_UTILS_H
#define MEGUMIN_MEGUMIN_UTILS_H

#include <string>
#include <optional>
#include <z3++.h>

#include "Program.h"

using std::string;
using namespace z3;

namespace megumin {
    void megumin_assert(bool expression, std::optional<string> message = {});

    [[noreturn]] void megumin_todo();

    [[noreturn]] void megumin_unreachable();

    std::optional<arm::Program> aarch64_asm(const std::string& code);

    string trim(const string& s);

    expr set_expr_range(expr x, int low, int high, int64_t value);
    expr set_expr_range(expr x, int low, int high, expr value);

    expr reverse_expr(const expr& x);

    expr z3_bool_to_bv(const expr& b, int size);

    expr count_leading_zero_64(const expr& x, int size);
    expr count_leading_zero_32(const expr& x, int size);
    expr count_leading_zero_16(const expr& x, int size);
    expr count_leading_zero_8(const expr& x, int size);
    expr count_leading_zero_4(const expr& x, int size);
    expr count_leading_zero_2(const expr& x, int size);
    expr count_leading_zero(int size, const expr& x);

//    expr count_leading_signed_64(const expr& x, int size);
//    expr count_leading_signed_32(const expr& x, int size);
//    expr count_leading_signed_16(const expr& x, int size);
//    expr count_leading_signed_8(const expr& x, int size);
//    expr count_leading_signed_4(const expr& x, int size);
//    expr count_leading_signed_2(const expr& x, int size);
    expr count_leading_signed(const expr& x, int size);

    expr round_up_to_next_power2_64(const expr& x);

    expr round_up_to_next_power2_32(const expr& x);
}

#endif //MEGUMIN_MEGUMIN_UTILS_H
