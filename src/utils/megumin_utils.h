//
// Created by 58413 on 2023/6/2.
//

#ifndef MEGUMIN_MEGUMIN_UTILS_H
#define MEGUMIN_MEGUMIN_UTILS_H

#include <string>
#include <optional>

#include "Program.h"

using std::string;

namespace megumin {
    void megumin_assert(bool expression, std::optional<string> message = {});

    void megumin_todo();

    std::optional<arm::Program> aarch64_asm(const std::string& code);

    string trim(const string& s);
}

#endif //MEGUMIN_MEGUMIN_UTILS_H
