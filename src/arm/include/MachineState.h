//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_MACHINESTATE_H
#define MEGUMIN_MACHINESTATE_H

#include "Bitvec.h"
#include "RegBank.h"

namespace arm {
    class PState {
    private:
        bool n = false;
        bool z = false;
        bool c = false;
        bool v = false;
        bool d = false;
        bool a = false;

    public:
         void set_nzcv(bits nzcv);
    };

    class MachineState {
    private:

    public:
        GPRegBank gp;
        PState p_state;

        MachineState();
        MachineState(const MachineState& other);

        void fill_gp_random();
    };
}

#endif //MEGUMIN_MACHINESTATE_H
