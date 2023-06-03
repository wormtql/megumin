//
// Created by 58413 on 2023/6/3.
//

#include "RegSet.h"

namespace arm {
    bool RegSet::have_gp(int index) {
        return gp.is_set(index);
    }

    bool RegSet::have_fp(int index) {
        return fp.is_set(index);
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