//
// Created by 58413 on 2023/4/14.
//

#include <cassert>

#include "MachineState.h"

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
    }
}