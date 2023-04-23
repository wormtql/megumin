//
// Created by 58413 on 2023/4/16.
//

#include <cstdint>
#include <cassert>

#include "ArmUtils.h"
#include "Bitvec.h"

// shared functions
namespace arm {
    std::pair<bits, bits> ArmUtilsSharedFunctions::lsr_c(bits x, int shift) {
        assert(shift > 0);
        int N = x.size;

        bool carry_out = x.is_set(shift - 1);
        uint64_t temp = static_cast<uint64_t>(x.data0) >> shift;
        bits result = bits{N, static_cast<int64_t>(temp)};

        return {result, bits{1, carry_out}};
    }

    bits ArmUtilsSharedFunctions::lsr(bits x, int shift) {
        assert(shift >= 0);
        if (shift == 0) {
            return x;
        } else {
            auto result = lsr_c(x, shift);
            return result.first;
        }
    }

    std::pair<bits, bits> ArmUtilsSharedFunctions::lsl_c(bits x, int shift) {
        assert(shift > 0);
        int N = x.size;

        int64_t temp = x.data0 << shift;
        bits result{N, temp};
        bool carry_out = x.is_set(N - shift);

        return {result, bits{1, carry_out}};
    }

    bits ArmUtilsSharedFunctions::lsl(bits x, int shift) {
        assert(shift >= 0);
        if (shift == 0) {
            return x;
        } else {
            auto result = lsl_c(x, shift);
            return result.first;
        }
    }

    std::pair<bits, bits> ArmUtilsSharedFunctions::ror_c(bits x, int shift) {
        assert(shift != 0);
        int N = x.size;
        int m = shift % N;
        auto result = lsr(x, m) | lsl(x, N - m);
        bool carry_out = result.is_set(N - 1);
        return {result, bits{1, carry_out}};
    }

    bits ArmUtilsSharedFunctions::ror(bits x, int shift) {
        assert(shift >= 0);
        if (shift == 0) {
            return x;
        } else {
            auto result = ror_c(x, shift);
            return result.first;
        }
    }

    std::pair<bits, bits> ArmUtilsSharedFunctions::asr_c(bits x, int shift) {
        assert(shift > 0);
        assert(shift < x.size);
        int64_t temp = x.sign_extend(64).data0;
        temp >>= shift;
        bool carry_out = x.is_set(shift - 1);
        return {bits{x.size, temp}, bits{1, carry_out}};
    }

    bits ArmUtilsSharedFunctions::asr(bits x, int shift) {
        assert(shift >= 0);
        if (shift == 0) {
            return x;
        } else {
            return asr_c(x, shift).first;
        }
    }
}

// instrs
namespace arm {
    bits ArmUtils::shift_reg(bits reg, int shift_type, int amount) {
        switch (shift_type) {
            case 0:
                return ArmUtilsSharedFunctions::lsl(reg, amount);
            case 1:
                return ArmUtilsSharedFunctions::lsr(reg, amount);
            case 2:
                return ArmUtilsSharedFunctions::asr(reg, amount);
            case 3:
                return ArmUtilsSharedFunctions::ror(reg, amount);
        }
    }

    bits ArmUtils::replicate(bits x, int m) {
        assert(x.size * m <= 64);
        int64_t result = 0;
        for (int i = 0; i < m; i++) {
            result = (result << x.size) | x.data0;
        }
        return bits{x.size * m, result};
    }

    bits ArmUtils::zero_extend(bits x, int size) {
        return x.zero_extend(size);
    }

    std::pair<bits, bits> ArmUtils::add_with_carry(bits x, bits y, bool carry) {
        assert(x.size == y.size);
        auto size = x.size;
        uint64_t unsigned_sum = x.as_u64() + y.as_u64() + static_cast<uint64_t>(carry);
        int64_t signed_sum = x.as_i64() + y.as_i64() + static_cast<int64_t>(carry);

        bits result{size, signed_sum};

        bool n = result.is_set(size - 1);
        bool z = result == 0;
        bool c = result.as_u64() != unsigned_sum;
        bool v = result.as_i64() != signed_sum;

        int temp = (n << 3) | (z << 2) | (c << 1) | v;
        bits nzcv{4, temp};
        return { result, nzcv };
    }

    int ArmUtils::highest_set_bit(bits x) {
        for (int i = x.size - 1; i >= 0; i--) {
            if (x.is_set(i)) {
                return i;
            }
        }
        return -1;
    }

    std::pair<bits, bits> ArmUtils::decode_bit_mask(int m, bool immN, bits imms, bits immr,
                                                    bool immediate) {

        int len = ArmUtils::highest_set_bit(bits{1, immN}.concat(~imms));
        assert(len >= 1);
        assert(m >= (1 << len));

        bits levels = bits{6, (1 << len) - 1};
        if (immediate && (imms & levels) == levels) {
            // undefined
            assert(false);
        }

        unsigned int S = (imms & levels).as_u32();
        unsigned int R = (immr & levels).as_u32();
        unsigned int diff = S - R;

        int esize = 1 << len;
        unsigned int d = bits{len, diff}.as_u32();
        bits welem = bits::ones(S + 1).zero_extend(esize);
        bits telem = bits::ones(d + 1).zero_extend(esize);
        bits wmask = replicate(ArmUtilsSharedFunctions::ror(welem, R), 64 / esize);
        bits tmask = replicate(telem, 64 / esize);
        return {wmask.resize(m), tmask.resize(m)};
    }
}
