//
// Created by 58413 on 2023/4/14.
//

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "MachineState.h"

using namespace std;

namespace arm {
    void PState::set_nzcv(bits nzcv) {
        assert(nzcv.size == 4);
        n = nzcv.is_set(3);
        z = nzcv.is_set(2);
        c = nzcv.is_set(1);
        v = nzcv.is_set(0);
    }

    MachineState::MachineState() {

    }

    std::mt19937 MachineState::generator{100};
    std::uniform_int_distribution<> MachineState::uniform_int;

    MachineState::MachineState(const MachineState &other) {
        this->gp = other.gp;
        this->fp = other.fp;
        this->p_state = other.p_state;
        this->fpcr = other.fpcr;
    }

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
}
