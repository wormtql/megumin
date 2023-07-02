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

        static expr replicate(const expr& x, int m);

        static expr shift_reg(const expr& reg, int shift_type, int amount);
        static expr shift_reg(const expr& reg, int shift_type, const expr& amount);
    };

    class ArmUtilSharedFunctionsS {
    public:
        static expr ror(const expr& x, int shift);
        static expr ror(const expr& x, const expr& shift);

        static std::pair<expr, expr> ror_c(const expr& x, int shift);
        static std::pair<expr, expr> ror_c(const expr& x, const expr& shift);

        static expr lsr(const expr& x, int shift);
        static expr lsr(const expr& x, const expr& shift);

        static std::pair<expr, expr> lsr_c(const expr& x, int shift);
//        static std::pair<expr, expr> lsr_c(const expr& x, const expr& shift);

        static expr lsl(const expr& x, int shift);
        static expr lsl(const expr& x, const expr& shift);

        static std::pair<expr, expr> lsl_c(const expr& x, int shift);
//        static std::pair<expr, expr> lsl_c(const expr& x, const expr& shift);

        static expr asr(const expr& x, int shift);
        static expr asr(const expr& x, const expr& shift);

        static std::pair<expr, expr> asr_c(const expr& x, int shift);
//        static std::pair<expr, expr> asr_c(const expr& x, const expr& shift);
    };
}



#endif //MEGUMIN_ARMUTILSS_H
