//
// Created by 58413 on 2023/5/15.
//

#ifndef MEGUMIN_MEGUMIN_UTILS_H
#define MEGUMIN_UTILS_H

#include <string>
#include <optional>

#include <Program.h>

#ifdef USE_KEYSTONE
#include <keystone/keystone.h>
#endif

namespace megumin {
#ifdef USE_KEYSTONE
    arm::Program aarch64_asm(const std::string& code);
#endif
}

#endif //MEGUMIN_MEGUMIN_UTILS_H
