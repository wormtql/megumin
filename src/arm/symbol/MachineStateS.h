//
// Created by 58413 on 2023/6/24.
//

#ifndef MEGUMIN_MACHINESTATES_H
#define MEGUMIN_MACHINESTATES_H

#include <z3++.h>
#include <random>
#include <utility>
#include <string>

#include "RegBankS.h"

using namespace z3;
using std::string;

namespace arm {
    struct PStateS {
        expr n;
        expr z;
        expr c;
        expr v;

        PStateS(expr n, expr z, expr c, expr v)
            : n(std::move(n)), z(std::move(z)), c(std::move(c)), v(std::move(v)) {}

        PStateS(z3::context& context, const string& prefix);

        void set_nzcv(const PStateS& s) {
            n = s.n;
            z = s.z;
            c = s.c;
            v = s.v;
        }

        expr operator==(const PStateS& other) const;
        expr operator!=(const PStateS& other) const;
    };

    class MachineStateS {
    private:
        z3::context& ctx;
    public:
        GPRegBankS gp;
        PStateS p_state;
        expr sp; // 64-bit sp

        MachineStateS(z3::context& context, const string& prefix);

        [[nodiscard]] expr get_sp(int size) const;
        void set_sp(int size, const expr& value);

        [[nodiscard]] expr get_gp(int size, int index, bool is_sp_31, bool is_zr_31) const;
        void set_gp(int size, int index, const expr& value, bool is_sp_31);

        expr operator==(const MachineStateS& other) const;
        expr operator!=(const MachineStateS& other) const;
    };
}



#endif //MEGUMIN_MACHINESTATES_H
