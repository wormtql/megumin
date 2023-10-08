//
// Created by 58413 on 2023/4/14.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "RegBank.h"

using std::endl;

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

namespace arm {
     std::ostream& operator<<(std::ostream& os, const arm::FPRegBank& reg) {
         for (int i = 0; i < 32; i++) {
             os << reg.get_ref(i, false).data0 << " " << reg.get_ref(i, true).data0 << endl;
         }

         return os;
     }

     std::ostream& operator<<(std::ostream& os, const arm::GPRegBank& reg) {
         for (int i = 0; i < 32; i++) {
             os << i << " " << reg.get_ref(i).data0 << endl;
         }

         return os;
     }
}
