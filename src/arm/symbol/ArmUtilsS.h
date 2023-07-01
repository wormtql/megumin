//
// Created by 58413 on 2023/6/27.
//

#ifndef MEGUMIN_ARMUTILSS_H
#define MEGUMIN_ARMUTILSS_H

#include <iostream>
#include <z3++.h>
#include "MachineStateS.h"
#include "Bitvec.h"

using namespace z3;

namespace arm {
    class ArmUtilsS {
    public:
        static std::pair<expr, PStateS> add_with_carry(expr x, expr y, expr carry);
        static std::pair<expr, PStateS> add_with_carry(const expr& x, bits y, bool carry);
    };
}



#endif //MEGUMIN_ARMUTILSS_H
