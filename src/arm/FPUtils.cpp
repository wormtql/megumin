//
// Created by 58413 on 2023/10/17.
//

#include "FPUtils.h"
#include "megumin_utils.h"

namespace arm {
    FPRounding FPUtils::fp_decode_rounding(int value) {
        if (value == 0) {
            return FPRounding::TIEEVEN;     // N
        } else if (value == 1) {
            return FPRounding::POSINF;      // P
        } else if (value == 2) {
            return FPRounding::NEGINF;      // M
        } else if (value == 3) {
            return FPRounding::ZERO;        // Z
        } else {
            megumin::megumin_assert(false);
        }

        // never
        return FPRounding::ZERO;
    }

    FPType FPUtils::get_fp_type(bits op) {
        int size = op.size;
        if (size == 32) {
            bits exponent = op[{23, 31}];
            bits mantissa = op[{0, 23}];

            if (exponent == 0 && mantissa == 0) {
                return FPType::Zero;
            } else if (exponent == 0 && mantissa != 0) {
                return FPType::Denormal;
            } else if (exponent == 0b11111111 && mantissa == 0) {
                return FPType::Infinity;
            } else if (exponent == 0b11111111 && mantissa[22] == 1) {
                return FPType::QNaN;
            } else if (exponent == 0b11111111 && mantissa[22] == 0 && mantissa != 0) {
                return FPType::SNaN;
            } else {
                return FPType::Normal;
            }
        } else if (size == 64) {
            bits exponent = op[{52, 63}];
            bits mantissa = op[{0, 52}];

            if (exponent == 0 && mantissa == 0) {
                return FPType::Zero;
            } else if (exponent == 0 && mantissa != 0) {
                return FPType::Denormal;
            } else if (exponent == 0b11111111111 && mantissa == 0) {
                return FPType::Infinity;
            } else if (exponent == 0b11111111111 && mantissa[51] == 1) {
                return FPType::QNaN;
            } else if (exponent == 0b11111111111 && mantissa[51] == 0 && mantissa != 0) {
                return FPType::SNaN;
            } else {
                return FPType::Normal;
            }
        } else {
            megumin::megumin_assert(false);
        }

        return FPType::SNaN;
    }

    bool FPUtils::get_fp_sign(bits op) {
        int size = op.size;
        return op.is_set(size - 1);
    }

    bits FPUtils::fp_round_int(bits op, FPRounding rounding, bool exact, FPException& exc) {
        int size = op.size;
        auto fp_type = FPUtils::get_fp_type(op);
        exc = FPException::None;

        if (fp_type == FPType::SNaN || fp_type == FPType::QNaN) {
            if (fp_type == FPType::SNaN) {
                // todo signal exception
                return bits::snan(size);
            } else {
                return bits::qnan(size);
            }
        }
        if (fp_type == FPType::Infinity) {
            return op;
        }
        if (fp_type == FPType::Zero) {
            return op;
        }

        if (size == 64) {
            double d = op.as_f64();
            double r = floor(d);
            double err = d - r;
            if (exact && err != 0.0) {
                exc = FPException::Inexact;
                return bits{64, 0};
            }

            if (rounding == FPRounding::TIEEVEN) {
                return bits{round(d)};
            } else if (rounding == FPRounding::POSINF) {
                return bits{ceil(d)};
            } else if (rounding == FPRounding::NEGINF) {
                return bits{floor(d)};
            } else if (rounding == FPRounding::ZERO) {
                bool sign = FPUtils::get_fp_sign(op);
                if (sign == 1) {
                    return bits{ceil(d)};
                } else {
                    return bits{floor(d)};
                }
            } else if (rounding == FPRounding::TIEAWAY) {
                // tie away from zero
                if (err > 0.5 || (err == 0.5 && r > 0)) {
                    r += 1;
                }
                return bits{r};
            }
        } else if (size == 32) {
            float d = op.as_f32();
            float r = floor(d);
            float err = d - r;
            if (exact && err != 0.0) {
                exc = FPException::Inexact;
                return bits{32, 0};
            }

            if (rounding == FPRounding::TIEEVEN) {
                return bits{round(d)};
            } else if (rounding == FPRounding::POSINF) {
                return bits{ceil(d)};
            } else if (rounding == FPRounding::NEGINF) {
                return bits{floor(d)};
            } else if (rounding == FPRounding::ZERO) {
                bool sign = FPUtils::get_fp_sign(op);
                if (sign == 1) {
                    return bits{ceil(d)};
                } else {
                    return bits{floor(d)};
                }
            } else if (rounding == FPRounding::TIEAWAY) {
                // tie away from zero
                if (err > 0.5 || (err == 0.5 && r > 0)) {
                    r += 1;
                }
                return bits{r};
            }
        }

        megumin::megumin_assert(false);
    }
}