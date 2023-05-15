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
        FPRegBank fp;
        PState p_state;
        bits fpcr{64, 0};

        MachineState();
        MachineState(const MachineState& other);

        void fill_gp_random();
        void fill_fp_random();

        [[nodiscard]] bool is_merging() const {
            return fpcr.is_set(2);
        }
    };
}

#endif //MEGUMIN_MACHINESTATE_H
