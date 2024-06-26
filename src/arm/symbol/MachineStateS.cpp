//
// Created by 58413 on 2023/6/24.
//

#include "MachineStateS.h"
#include <megumin_utils.h>

using namespace std;

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

    PState PStateS::to_p_state(const z3::model &m) const {
        PState result;

        result.n = m.eval(this->n, true).bool_value() == Z3_L_TRUE;
        result.z = m.eval(this->z, true).bool_value() == Z3_L_TRUE;
        result.c = m.eval(this->c, true).bool_value() == Z3_L_TRUE;
        result.v = m.eval(this->v, true).bool_value() == Z3_L_TRUE;
//        result.n = m.eval(this->n, true).get_numeral_uint64();
//        result.z = m.eval(this->z, true).get_numeral_uint64();
//        result.c = m.eval(this->c, true).get_numeral_uint64();
//        result.v = m.eval(this->v, true).get_numeral_uint64();
        return result;
    }

    PStateS PStateS::from_pstate_literal(z3::context& context, const PState &lit) {
        PStateS ret{context, "temp"};
        ret.n = context.bool_val(lit.n);
        ret.z = context.bool_val(lit.z);
        ret.c = context.bool_val(lit.c);
        ret.v = context.bool_val(lit.v);

        return ret;
    }
}

namespace arm {
    MachineStateS::MachineStateS(z3::context& context, const string& prefix)
        : ctx(context),
          p_state(context, prefix + "_ps"),
          gp(context, prefix + "_gp"),
          sp(context.bv_const((prefix + "_sp").c_str(), 64))
    {
//        cout << sp.get_sort() << endl;
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

    MachineState MachineStateS::to_machine_state(const model &m) const {
        MachineState result;

        result.p_state = p_state.to_p_state(m);
        result.gp = gp.to_gp_reg_bank(m);
        result.sp = bits{64, (int64_t) m.eval(sp, true).get_numeral_uint64()};

        return result;
    }

    MachineStateS MachineStateS::from_machine_state_literal(z3::context &context, const MachineState &lit) {
        MachineStateS ret{context, "temp"};
        ret.p_state = PStateS::from_pstate_literal(context, lit.p_state);
        ret.gp = GPRegBankS::from_gp_reg_bank_literal(context, lit.gp);
        ret.sp = context.bv_val(lit.sp.data0, lit.sp.size);

        return ret;
    }
}
