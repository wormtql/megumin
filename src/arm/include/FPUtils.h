#ifndef MEGUMIN_FPUTILS_H
#define MEGUMIN_FPUTILS_H

#include "Bitvec.h"

namespace arm {
    enum class FPType {
        Zero,       // exponent = 0, mantissa = 0
        Denormal,   // exponent = 0, mantissa != 0, 0.xxxx
        Normal,    // exponent = [1, 254]
        Infinity,   // exponent = 255, mantissa = 0
        QNaN,       // exponent = 255, mantissa = 0b1000...
        SNaN,       // exponent = 255, mantissa = 0b0100...
    };

    enum class FPRounding {
        TIEEVEN,
        POSINF,
        NEGINF,
        ZERO,
        TIEAWAY,
        ODD,
    };

    enum class FPException {
        None,
        InvalidOp,
        DivideByZero,
        Overflow,
        Underflow,
        Inexact,
        InputDenorm,
    };

    class FPUtils {
    public:
        static FPType get_fp_type(bits op);

        static bool get_fp_sign(bits op);

        static bool is_floor_even(double x);
        static bool is_floor_even(float x);

        static FPRounding fp_decode_rounding(int value);

        // float to nearest integral **float** （不是round到整数，还是round到IEEE浮点）
        static bits fp_round_int(bits op, FPRounding rounding, bool exact, FPException& exc);

        static bits fp_process_nan(FPType ty, bits op, FPException& exc);

        static std::pair<bool, bits> fp_process_nans(FPType type1, FPType type2, bits op1, bits op2, bits fpcr, bool altfmaxmin, FPException& exc);

        static bits fp_sqrt(bits op, FPException& exc);

        static bits vfp_expand_imm(int size, bits imm8);

        static bits fp_max(bits op1, bits op2, bool altfp, FPException& exc);

        static bits fp_min(bits op1, bits op2, bool altfp, FPException& exc);

        // return nzcv
        static bits fp_compare(bits op1, bits op2, bool signal_nans, FPException& exc);
    };
}

#endif //MEGUMIN_FPUTILS_H
