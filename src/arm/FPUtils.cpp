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

    bool FPUtils::is_floor_even(float x) {
        bits b{x};
        bits mantissa = b.get_range(0, 23);
        bits exponent = b.get_range(23, 31);
        bool sign = b.is_set(31);

        auto ty = FPUtils::get_fp_type(b);
        if (ty == FPType::Denormal) {
            // +0.xxxx -> 0  -0.xxxx -> -1
            return sign == 0;
        }
        if (ty == FPType::Zero) {
            return true;
        }
        if (ty == FPType::SNaN || ty == FPType::QNaN || ty == FPType::Infinity) {
            return false;
        }

        int real_exponent = exponent.as_i32() - 127;
        if (real_exponent < 0) {
            return sign == 0;
        } else if (real_exponent == 0) {
            // 1.xxxx -> 1   -1.xxxx -> -2
            return sign == 1;
        } else {
            if (real_exponent > 23) {
                return true;
            } else {
                bool bit = b.is_set(23 - real_exponent);
                if (sign == 0) {
                    return bit == 0;
                } else {
                    return bit == 1;
                }
            }
        }
    }

    bool FPUtils::is_floor_even(double x) {
        bits b{x};
        bits mantissa = b.get_range(0, 52);
        bits exponent = b.get_range(52, 63);
        bool sign = b.is_set(63);

        auto ty = FPUtils::get_fp_type(b);
        if (ty == FPType::Denormal) {
            // +0.xxxx -> 0  -0.xxxx -> -1
            return sign == 0;
        }
        if (ty == FPType::Zero) {
            return true;
        }
        if (ty == FPType::SNaN || ty == FPType::QNaN || ty == FPType::Infinity) {
            return false;
        }

        int real_exponent = exponent.as_i32() - 1023;
        if (real_exponent < 0) {
            return sign == 0;
        } else if (real_exponent == 0) {
            // 1.xxxx -> 1   -1.xxxx -> -2
            return sign == 1;
        } else {
            if (real_exponent > 52) {
                return true;
            } else {
                bool bit = b.is_set(52 - real_exponent);
                if (sign == 0) {
                    return bit == 0;
                } else {
                    return bit == 1;
                }
            }
        }
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
                bool even = FPUtils::is_floor_even(d);
                if (err > 0.5 || (err == 0.5 && !even)) {
                    return bits{r + 1};
                }
                return bits{r};
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
                bool even = FPUtils::is_floor_even(d);
                if (err > 0.5 || (err == 0.5 && !even)) {
                    return bits{r + 1};
                }
                return bits{r};
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

    bits FPUtils::fp_process_nan(FPType ty, bits op, FPException& exc) {
        if (ty == FPType::SNaN) {
            exc = FPException::InvalidOp;
        }
        return op;
    }

    bits FPUtils::fp_sqrt(bits op, FPException& exc) {
        auto ty = FPUtils::get_fp_type(op);
        auto sign = FPUtils::get_fp_sign(op);
        int size = op.size;

        if (ty == FPType::SNaN || ty == FPType::QNaN) {
            return FPUtils::fp_process_nan(ty, op, exc);
        } else if (ty == FPType::Zero) {
            return op;
        } else if (ty == FPType::Infinity) {
            if (sign == 0) {
                return op;
            } else {
                exc = FPException::InvalidOp;
                return bits::snan(size);
            }
        } else {
            if (sign == 1) {
                exc = FPException::InvalidOp;
                return bits::snan(size);
            }

            if (size == 64) {
                double value = op.as_f64();
                value = sqrt(value);
                return bits{value};
            } else if (size == 32) {
                float value = op.as_f32();
                value = sqrt(value);
                return bits{value};
            } else {
                megumin::megumin_assert(false);
            }
        }

        return bits{size, 0};
    }

    bits FPUtils::vfp_expand_imm(int size, bits imm8) {
        int e = 0;
        int f = 0;
        if (size == 32) {
            e = 8;
            f = 32 - 8 - 1;
        } else if (size == 64) {
            e = 11;
            f = 64 - 11 - 1;
        } else {
            megumin::megumin_assert(false);
        }

        bool sign = imm8.is_set(7);
        bits exp = bits::from_bools({!imm8.is_set(6)});
        for (int i = 0; i < e - 3; i++) {
            exp = exp.concat(bits::from_bools({imm8.is_set(6)}));
        }
        exp = exp.concat(imm8.get_range(4, 6));

        bits frac = imm8.get_range(0, 4).concat(bits{f - 4, 0});
        bits result = bits::from_bools({sign}).concat(exp).concat(frac);

        return result;
    }

    std::pair<bool, bits> FPUtils::fp_process_nans(FPType type1, FPType type2, bits op1, bits op2, bits fpcr,
                                                   bool altfmaxmin, FPException &exc) {
        // to be simple raise error when any is sNaN

        bool op1_nan = type1 == FPType::SNaN || type1 == FPType::QNaN;
        bool op2_nan = type2 == FPType::SNaN || type2 == FPType::QNaN;
        bool any_snan = type1 == FPType::SNaN || type2 == FPType::SNaN;

        if (type1 == FPType::SNaN || type2 == FPType::SNaN) {
            exc = FPException::InvalidOp;
            return {true, bits::snan(op1.size)};
        }
        if (op1_nan || op2_nan) {
            return {true, bits::qnan(op1.size)};
        }

        return {false, op1};

//        bool op1_nan = type1 == FPType::SNaN || type1 == FPType::QNaN;
//        bool op2_nan = type2 == FPType::SNaN || type2 == FPType::QNaN;
//        bool any_snan = type1 == FPType::SNaN || type2 == FPType::SNaN;
//        int size = op1.size;
//
//        bool done = false;
//        bool sign2;
//        bits result;
//
//        if (altfmaxmin && (op1_nan || op2_nan)) {
//            exc = FPException::InvalidOp;
//            done = true;
//            sign2 = op2.is_set(op2.size - 1);
//            result = type2 == FPType::Zero ? bits::fpzero(sign2, size) : op2;
//        } else if (altfp)
    }

    bits FPUtils::fp_max(bits op1, bits op2, bool altfp, FPException& exc) {
        FPType type1 = FPUtils::get_fp_type(op1);
        FPType type2 = FPUtils::get_fp_type(op2);
        bool sign1 = FPUtils::get_fp_sign(op1);
        bool sign2 = FPUtils::get_fp_sign(op2);

        if (altfp && type1 == FPType::Zero && type2 == FPType::Zero && sign1 + sign2 == 1) {
            return bits::fpzero(sign2, op1.size);
        }

        auto r = FPUtils::fp_process_nans(type1, type2, op1, op2, bits{64, 0}, false, exc);
        bool done = r.first;
        bits result = r.second;

        if (done) {
            return result;
        }

        int size = op1.size;
        if (size == 64) {
            double d1 = op1.as_f64();
            double d2 = op2.as_f64();
            double m = fmax(d1, d2);
            return bits{m};
        } else if (size == 32) {
            float d1 = op1.as_f32();
            float d2 = op2.as_f32();
            float m = fmax(d1, d2);
            return bits{m};
        } else {
            megumin::megumin_assert(false);
        }

        return bits::snan(size);
    }

    bits FPUtils::fp_min(bits op1, bits op2, bool altfp, FPException &exc) {
        FPType type1 = FPUtils::get_fp_type(op1);
        FPType type2 = FPUtils::get_fp_type(op2);
        bool sign1 = FPUtils::get_fp_sign(op1);
        bool sign2 = FPUtils::get_fp_sign(op2);

        if (altfp && type1 == FPType::Zero && type2 == FPType::Zero && sign1 + sign2 == 1) {
            return bits::fpzero(sign2, op1.size);
        }

        auto r = FPUtils::fp_process_nans(type1, type2, op1, op2, bits{64, 0}, false, exc);
        bool done = r.first;
        bits result = r.second;

        if (done) {
            return result;
        }

        int size = op1.size;
        if (size == 64) {
            double d1 = op1.as_f64();
            double d2 = op2.as_f64();
            double m = fmin(d1, d2);
            return bits{m};
        } else if (size == 32) {
            float d1 = op1.as_f32();
            float d2 = op2.as_f32();
            float m = fmin(d1, d2);
            return bits{m};
        } else {
            megumin::megumin_assert(false);
        }

        return bits::snan(size);
    }
}
