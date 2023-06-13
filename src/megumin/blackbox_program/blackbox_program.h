//
// Created by 58413 on 2023/6/10.
//

#ifndef MEGUMIN_BLACKBOX_PROGRAM_H
#define MEGUMIN_BLACKBOX_PROGRAM_H

#include "MachineState.h"

namespace megumin::blackbox {
    arm::MachineState swap(const arm::MachineState& state);

    arm::MachineState add10(const arm::MachineState& state);
}


#endif //MEGUMIN_BLACKBOX_PROGRAM_H
