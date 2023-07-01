//
// Created by 58413 on 2023/6/24.
//

#include "MachineStateS.h"
#include <megumin_utils.h>

// pstate
namespace arm {
    PStateS::PStateS(z3::context &context, const string& prefix)
        : n(context.bool_const((prefix + "_n").c_str())),
          z(context.bool_const((prefix + "_z").c_str())),
          c(context.bool_const((prefix + "_c").c_str())),
          v(context.bool_const((prefix + "_v").c_str()))
    {
    }

    expr PStateS::operator==(const PStateS &other) const {
        return n == other.n && z == other.z && c == other.c && v == other.v;
    }

    expr PStateS::operator!=(const PStateS &other) const {
        return !(*this == other);
    }
}

namespace arm {
    MachineStateS::MachineStateS(z3::context& context, const string& prefix)
        : ctx(context),
          p_state(context, prefix + "_ps"),
          gp(context, prefix + "_gp"),
          sp(context.bv_const((prefix + "_sp").c_str(), 64))
    {

    }

    expr MachineStateS::get_sp(int size) const {
        if (size == 32) {
            return sp.extract(31, 0);
        } else if (size == 64) {
            return sp;
        } else {
            megumin::megumin_assert(false);
        }
    }

    void MachineStateS::set_sp(int size, const expr &value) {
        if (size == 64) {
            sp = value;
        } else if (size == 32) {
            sp = z3::zext(value, 32);
        } else {
            megumin::megumin_assert(false);
        }
    }

    expr MachineStateS::get_gp(int size, int index, bool is_sp_31, bool is_zr_31) const {
        if (index == 31) {
            if (is_sp_31) {
                return get_sp(size);
            } else if (is_zr_31) {
                return gp.get(size, 31);
            } else {
                return gp.get(size, 31);
            }
        } else {
            return gp.get(size, index);
        }
    }

    void MachineStateS::set_gp(int size, int index, const expr &value, bool is_sp_31) {
        if (index == 31) {
            if (is_sp_31) {
                set_sp(size, value);
            }
        } else {
            if (size == 64) {
                gp.set(index, value);
            } else if (size == 32) {
                gp.set(index, z3::zext(value, 32));
            } else {
                megumin::megumin_assert(false);
            }
        }
    }

    expr MachineStateS::operator==(const MachineStateS &other) const {
        return gp == other.gp && p_state == other.p_state && sp == other.sp;
    }

    expr MachineStateS::operator!=(const MachineStateS &other) const {
        return !(*this == other);
    }
}
