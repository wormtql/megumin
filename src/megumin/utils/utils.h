//
// Created by 58413 on 2023/5/15.
//

#ifndef MEGUMIN_UTILS_H
#define MEGUMIN_UTILS_H

#include <string>
#include <optional>

#include <Program.h>
#include <keystone/keystone.h>

namespace megumin {
    std::optional<arm::Program> aarch64_asm(const std::string& code);
}

#endif //MEGUMIN_UTILS_H
