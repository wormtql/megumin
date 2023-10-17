//
// Created by 58413 on 2023/4/14.
//

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "MachineState.h"
#include "megumin_utils.h"

using namespace std;

namespace arm {
    void PState::set_nzcv(bits nzcv) {
        assert(nzcv.size == 4);
        n = nzcv.is_set(3);
        z = nzcv.is_set(2);
        c = nzcv.is_set(1);
        v = nzcv.is_set(0);
    }

    bool PState::operator==(const PState &other) const {
        return n == other.n && c == other.c && z == other.z && v == other.v && d == other.d && a == other.a;
    }

    bool PState::operator!=(const PState &other) const {
        return !(other == *this);
    }

    std::mt19937 MachineState::generator{999};
    std::uniform_int_distribution<> MachineState::uniform_int;

//    MachineState::MachineState(const MachineState &other) {
//        this->gp = other.gp;
//        this->fp = other.fp;
//        this->p_state = other.p_state;
//        this->fpcr = other.fpcr;
//        this->
//    }

    void MachineState::fill_gp_random() {
        // don't set x31, which is xzr, which is always zero
        for (int i = 0; i < 31; i++) {
            auto r = (int64_t) uniform_int(generator);
            r = (r << 32) | uniform_int(generator);
            gp.get_mut_ref(i).set_value(r);
            // cout << gp.get_ref(i).as_u64() << endl;
        }
    }

    void MachineState::fill_fp_random() {
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 2; j++) {
                auto temp = (int64_t) uniform_int(generator);
                int64_t r = (temp << 32) | (int64_t) uniform_int(generator);
                fp.get_mut_ref(i, j).set_value(r);
            }
        }
    }

    void MachineState::fill_nzcv_random() {
        p_state.n = r() % 2;
        p_state.z = r() % 2;
        p_state.c = r() % 2;
        p_state.v = r() % 2;
    }

    void MachineState::fill_sp_random() {
        auto r = (int64_t) uniform_int(generator);
        r = (r << 32) | uniform_int(generator);
        sp.set_value(r);
    }

    bits MachineState::get_sp(int size) const {
        return sp.resize(size);
    }

    void MachineState::set_sp(int size, const bits& value) {
        if (size == 32) {
            sp.set_value(value.resize(32).zero_extend(64));
        } else if (size == 64) {
            sp.set_value(value.resize(64));
        }
    }

    bits MachineState::get_gp(int size, int index, bool is_sp_31, bool is_zr_31) const {
        if (index == 31) {
            if (is_sp_31) {
                return get_sp(size);
            } else if (is_zr_31) {
                return gp.get(size, index);
            } else {
                megumin::megumin_assert(false);
            }
        } else {
            return gp.get(size, index);
        }

        return {};
    }

    void MachineState::set_gp(int size, int index, const bits& value, bool is_sp_31) {
        if (index == 31) {
            if (is_sp_31) {
                set_sp(size, value);
            }
        } else {
            gp.set(size, index, value);
        }
    }

    void MachineState::fill_random() {
        fill_fp_random();
        fill_gp_random();
        fill_nzcv_random();
        fill_sp_random();
    }

    bool MachineState::operator==(const MachineState &other) const {
        if (p_state != other.p_state) {
            return false;
        }

        if (gp != other.gp) {
            return false;
        }

        if (fp != other.fp) {
            return false;
        }

        if (sp != other.sp) {
            return false;
        }

        return true;
    }

    bool MachineState::operator!=(const MachineState &other) const {
        return !(*this == other);
    }

    FPRounding MachineState::get_rounding_mode() const {
        bits rmode = this->fpcr[{22, 24}];
        return FPUtils::fp_decode_rounding(rmode.as_i32());
    }
}

namespace arm {
    std::ostream& operator<<(std::ostream& os, const arm::PState& p_state) {
        os << "n = " << p_state.n;
        os << "z = " << p_state.z;
        os << "c = " << p_state.c;
        os << "v = " << p_state.v;

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const MachineState& ms) {
        os << "fp =" << endl << ms.fp << endl;
        os << "gp =" << endl << ms.gp << endl;
        os << "p_state = " << endl << ms.p_state << endl;
        os << "fpcr = " << ms.fpcr.data0 << endl;
        os << "sp = " << ms.sp.data0;

        return os;
    }
}