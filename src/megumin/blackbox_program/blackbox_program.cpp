//
// Created by 58413 on 2023/6/10.
//

#include "blackbox_program.h"

namespace megumin::blackbox {
    arm::MachineState swap(const arm::MachineState& state) {
        arm::MachineState result{state};

        auto temp = result.gp.get_ref(1);
        result.gp.set64(1, result.gp.get_ref(2));
        result.gp.set64(2, temp);

        return result;
    }

    arm::MachineState add10(const arm::MachineState& state) {
        arm::MachineState result{state};
        auto temp = result.gp.get_ref(1);
        auto r = temp.data0 + 10;
        result.gp.get_mut_ref(1).data0 = r;
        return result;
    }
}