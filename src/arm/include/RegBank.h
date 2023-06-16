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

        void set16_merge(int index, bits data) {
            bank[index * 2 + 1].set_range(0, 16, data.data0);
        }

        void set32_merge(int index, bits data) {
            bank[index * 2 + 1].set_range(0, 32, data.data0);
        }

        void set64_merge(int index, bits data) {
            bank[index * 2 + 1].set_value(data.resize(64));
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

        void set(int size, int index, bits data, bool merge = false) {
            if (size == 16) {
                merge ? set16_merge(index, data): set16(index, data);
            } else if (size == 32) {
                merge ? set32_merge(index, data) : set32(index, data);
            } else if (size == 64) {
                merge ? set64_merge(index, data) : set64(index, data);
            } else {
                assert(false);
            }
        }

        [[nodiscard]] const bits& get_ref(int index, bool low) const {
            int i = low ? index * 2 + 1 : index * 2;
            return bank[i];
        }

        [[nodiscard]] bits& get_mut_ref(int index, bool low) {
            int i = low ? index * 2 + 1 : index * 2;
            return bank[i];
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

        [[nodiscard]] bits get32(int index) const {
//            return bank[index] & ((1ll << 32) - 1);
            return bank[index].resize(32);
        }

        [[nodiscard]] bits get64(int index) const {
            return bank[index];
        }

        [[nodiscard]] bits get(int size, int index) const {
            if (size == 32) {
                return get32(index);
            } else if (size == 64) {
                return get64(index);
            }
            assert(false);
        }

        // will zero high 32 bits
        void set32(int index, bits data) {
            if (index == 31) {
                return;
            }
            bank[index].set_value(data.resize(32).zero_extend(64));
        };
        void set64(int index, bits data) {
            if (index == 31) {
                return;
            }
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
