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

    std::optional<arm::Program> aarch64_asm(const std::string& code);

    string trim(const string& s);

    expr set_expr_range(expr x, int low, int high, int64_t value);
}

#endif //MEGUMIN_MEGUMIN_UTILS_H
