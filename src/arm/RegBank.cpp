//
// Created by 58413 on 2023/4/14.
//

#include <cstdlib>
#include <iostream>
#include "RegBank.h"

namespace arm {
    GPRegBank::GPRegBank() {
        for (int i = 0; i < 32; i++) {
            bank[i] = bits{64, 0};
        }
    }

    GPRegBank::GPRegBank(const GPRegBank &other) {
        memcpy(bank, other.bank, sizeof(bank));
    }

    FPRegBank::FPRegBank() {
        for (int i = 0; i < 64; i++) {
            bank[i] = bits{64, 0};
        }
    }

    FPRegBank::FPRegBank(const FPRegBank& other) {
        memcpy(bank, other.bank, sizeof(bank));
    }
}
