//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_REGBANK_H
#define MEGUMIN_REGBANK_H

#include <cstdint>
#include <cassert>
#include "Bitvec.h"

namespace arm {
    class FPRegBank {
    private:
        // 32 128-bit registers
        bits bank[64]{};
    public:
        FPRegBank();
        FPRegBank(const FPRegBank& other);

        bits get16(int index) {
            return bank[index * 2 + 1].resize(16);
        }

        bits get32(int index) {
            return bank[index * 2 + 1].resize(32);
        }

        bits get64(int index) {
            return bank[index * 2 + 1];
        }

        bits get(int size, int index) {
            if (size == 32) {
                return get32(index);
            } else if (size == 64) {
                return get64(index);
            } else if (size == 16) {
                return get16(index);
            }
            assert(false);
        }

        void set16(int index, bits data) {
            bank[index * 2 + 1].set_value(data.resize(16).zero_extend(64));
            bank[index * 2].clear();
        }

        void set32(int index, bits data) {
            bank[index * 2 + 1].set_value(data.resize(32).zero_extend(64));
            bank[index * 2].clear();
        }

        void set64(int index, bits data) {
            bank[index * 2 + 1].set_value(data.resize(64));
            bank[index * 2].clear();
        }

        void set(int size, int index, bits data) {
            if (size == 16) {
                set16(index, data);
            } else if (size == 32) {
                set32(index, data);
            } else if (size == 64) {
                set64(index, data);
            } else {
                assert(false);
            }
        }
    };

    class GPRegBank {
    private:
         bits bank[32]{};
    public:
        GPRegBank();
        GPRegBank(const GPRegBank& other);

        [[nodiscard]] const bits& get_ref(int index) const {
            return bank[index];
        }

        [[nodiscard]] bits& get_mut_ref(int index) {
            return bank[index];
        }

        bits get32(int index) {
//            return bank[index] & ((1ll << 32) - 1);
            return bank[index].resize(32);
        }

        bits get64(int index) {
            return bank[index];
        }

        bits get(int size, int index) {
            if (size == 32) {
                return get32(index);
            } else if (size == 64) {
                return get64(index);
            }
            assert(false);
        }

        // will zero high 32 bits
        void set32(int index, bits data) {
            bank[index].set_value(data.resize(32).zero_extend(64));
        };
        void set64(int index, bits data) {
            bank[index].set_value(data.resize(64));
        }

        void set(int size, int index, const bits& data) {
            if (size == 32) {
                set32(index, data);
            } else if (size == 64) {
                set64(index, data);
            }
        }
    };
}



#endif //MEGUMIN_REGBANK_H
