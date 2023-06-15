//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_MACHINESTATE_H
#define MEGUMIN_MACHINESTATE_H

#include <random>
#include "Bitvec.h"
#include "RegBank.h"

namespace arm {
    class PState {
    public:
        bool n = false;
        bool z = false;
        bool c = false;
        bool v = false;
        bool d = false;
        bool a = false;

    public:
        void set_nzcv(bits nzcv);
        [[nodiscard]] bool get_c() const { return c; }
    };

    class MachineState {
    private:
        static std::mt19937 generator;
        static std::uniform_int_distribution<> uniform_int;

        static std::uniform_int_distribution<>::result_type r() {
            return uniform_int(generator);
        }
    public:
        GPRegBank gp;
        FPRegBank fp;
        PState p_state;
        bits fpcr{64, 0};

        MachineState();
        MachineState(const MachineState& other);

        void fill_gp_random();
        void fill_fp_random();
        void fill_nzcv_random();

        [[nodiscard]] bool is_merging() const {
            return fpcr.is_set(2);
        }
    };
}

#endif //MEGUMIN_MACHINESTATE_H
