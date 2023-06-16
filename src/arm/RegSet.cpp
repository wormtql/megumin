//
// Created by 58413 on 2023/6/3.
//

#include "RegSet.h"
#include "megumin_utils.h"

// RegSet
namespace arm {
    bool RegSet::have_gp(int index) const {
        return gp.is_set(index);
    }

    bool RegSet::have_fp(int index) const {
        return fp.is_set(index);
    }

    bool RegSet::have_reg(const Reg &reg) const {
        if (reg.reg_type == RegType::FP) {
            return have_fp(reg.index);
        } else if (reg.reg_type == RegType::GP) {
            return have_gp(reg.index);
        }
        return false;
    }

    void RegSet::set_fp(int index, bool value) {
        fp.set_bit(index, value);
    }

    void RegSet::set_gp(int index, bool value) {
        gp.set_bit(index, value);
    }

    RegSet RegSet::operator|(const Reg &other) {
        RegSet result = *this;
        if (other.reg_type == RegType::GP) {
            result.gp.set_bit(other.index, true);
        } else if (other.reg_type == RegType::FP) {
            result.fp.set_bit(other.index, true);
        }
        return result;
    }

    void RegSet::add_reg(const Reg &other) {
        if (other.reg_type == RegType::GP) {
            gp.set_bit(other.index, true);
        } else if (other.reg_type == RegType::FP) {
            fp.set_bit(other.index, true);
        }
    }

    int RegSet::random_gp(std::mt19937& generator) const {
        int arr[32];
        int size = 0;
        uint64_t temp = (uint64_t) gp.data0;

        int it = 0;
        while (temp > 0) {
            if (temp % 2 == 1) {
                arr[size++] = it;
            }
            it++;
            temp >>= 1;
        }
        // add xzr/wzr
        arr[size++] = 31;
        megumin::megumin_assert(size > 0, "gp reg set size is 0");

        std::uniform_int_distribution<> uniform_int;
        int index = uniform_int(generator) % size;
        return arr[index];
    }

    int RegSet::random_fp(std::mt19937 &generator) const {
        int arr[32];
        int size = 0;
        uint64_t temp = (uint64_t) fp.data0;

        int it = 0;
        while (temp > 0) {
            if (temp % 2 == 1) {
                arr[size++] = it;
            }
            it++;
            temp >>= 1;
        }
        megumin::megumin_assert(size > 0, "fp reg set size is 0");

        std::uniform_int_distribution<> uniform_int;
        int index = uniform_int(generator) % size;
        return arr[index];
    }
}

namespace arm {
    Reg::Reg() {
        reg_type = RegType::None;
        index = 0;
    }

    Reg::Reg(RegType reg_type, int index)
            :reg_type(reg_type), index(index)
    {}

    Reg Reg::gp(int index) {
        return Reg{RegType::GP, index};
    }

    Reg Reg::fp(int index) {
        return Reg{RegType::FP, index};
    }
}

std::ostream& operator<<(std::ostream& os, const arm::RegSet& reg_set) {
    os << "gp: ";
    for (int i = 0; i < 32; i++) {
        if (reg_set.have_gp(i)) {
            os << i << ", ";
        }
    }
    os << std::endl;
    os << "fp: ";
    for (int i = 0; i < 32; i++) {
        if (reg_set.have_fp(i)) {
            os << i << ", ";
        }
    }
    return os;
}
