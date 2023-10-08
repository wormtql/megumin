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
        bool operator==(const PState& other) const;
        bool operator!=(const PState& other) const;
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
        bits sp{64, 0};

        MachineState() = default;
//        MachineState(const MachineState& other);

        void fill_gp_random();
        void fill_fp_random();
        void fill_nzcv_random();
        void fill_sp_random();
        void fill_random();

        [[nodiscard]] bits get_sp(int size) const;
        void set_sp(int size, const bits& value);

        [[nodiscard]] bits get_gp(int size, int index, bool is_sp_31, bool is_zr_31) const;
        void set_gp(int size, int index, const bits& value, bool is_sp_31);

        [[nodiscard]] bool is_merging() const {
            return fpcr.is_set(2);
        }

        bool operator==(const MachineState& other) const;
        bool operator!=(const MachineState& other) const;
    };

    std::ostream& operator<<(std::ostream& os, const arm::PState& p_state);
    std::ostream& operator<<(std::ostream& os, const arm::MachineState& ms);
}

#endif //MEGUMIN_MACHINESTATE_H
