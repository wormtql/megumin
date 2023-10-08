//
// Created by 58413 on 2023/4/14.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
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

    bool GPRegBank::operator==(const GPRegBank &other) const {
        for (int i = 0; i < 32; i++) {
            if (bank[i] != other.bank[i]) {
                return false;
            }
        }
        return true;
    }

    bool GPRegBank::operator!=(const GPRegBank &other) const {
        return !(*this == other);
    }

    FPRegBank::FPRegBank() {
        for (int i = 0; i < 64; i++) {
            bank[i] = bits{64, 0};
        }
    }

    FPRegBank::FPRegBank(const FPRegBank& other) {
        memcpy(bank, other.bank, sizeof(bank));
    }

    bool FPRegBank::operator==(const FPRegBank &other) const {
        for (int i = 0; i < 32; i++) {
            if (bank[i] != other.bank[i]) {
                return false;
            }
        }
        return true;
    }

    bool FPRegBank::operator!=(const FPRegBank &other) const {
        return !(*this == other);
    }
}

namespace std {
    // ostream& operator<<(ostream& os, arm::FPRegBank reg) {
    //     for (int i = 0; i < 32; i++) {
    //         os << reg.get_ref(i, true)
    //     }
    // }
}