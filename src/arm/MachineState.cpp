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

    MachineState::MachineState(const MachineState &other) {
        this->gp = other.gp;
        this->fp = other.fp;
        this->p_state = other.p_state;
        this->fpcr = other.fpcr;
    }

    void MachineState::fill_gp_random() {
        for (int i = 0; i < 32; i++) {
            auto r = (int64_t) rand();
            r = (r << 32) | rand();
            gp.get_mut_ref(i).set_value(r);
            // cout << gp.get_ref(i).as_u64() << endl;
        }
    }

    void MachineState::fill_fp_random() {
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 2; j++) {
                auto r = (int64_t) rand();
//                double d = *reinterpret_cast<double*>(&r);
                fp.get_mut_ref(i, j).set_value(r);
            }
        }
    }
}