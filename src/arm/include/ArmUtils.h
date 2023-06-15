//
// Created by 58413 on 2023/4/16.
//

#ifndef MEGUMIN_ARMUTILS_H
#define MEGUMIN_ARMUTILS_H

#include <vector>
#include <cassert>

#include "Bitvec.h"
#include "MachineState.h"

namespace arm {
    class ArmUtils {
    public:
        static std::pair<bits, bits> add_with_carry(bits x, bits y, bool carry);

        static int highest_set_bit(bits x);

        static std::pair<bits, bits> decode_bit_mask(int m, bool immN, bits imms, bits immr, bool immediate);

        static bits zero_extend(bits x, int size);

        static bits replicate(bits x, int m);

        static bits shift_reg(bits reg, int shift_type, int amount);
    };

    class ArmUtilsSharedFunctions {
    public:
        static bits lsr(bits x, int shift);

        static std::pair<bits, bits> lsr_c(bits x, int shift);

        static bits lsl(bits x, int shift);

        static std::pair<bits, bits> lsl_c(bits x, int shift);

        static bits ror(bits x, int shift);

        static std::pair<bits, bits> ror_c(bits x, int shift);

        static bits asr(bits x, int shift);

        static std::pair<bits, bits> asr_c(bits x, int shift);

        static int count_leading_zero_bits(const bits& x);

        static int count_leading_sign_bits(const bits& x);

        static bool condition_holds(int cond, const MachineState& state);
    };
}

#endif //MEGUMIN_ARMUTILS_H
