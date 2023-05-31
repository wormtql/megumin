#ifndef MYFLOAT_H
#define MYFLOAT_H

#include "Bitvec.h"

namespace arm {
    class bits;

    class Float {
    private:
        union {
            double double_value;
            float float_value;
        } value;
        // 0: double, 1: float, 2: half
        int type;

    public:
        Float(const bits& bs);
        explicit Float(int size);
        Float(double value);
        Float(float value);

        bits to_bits() const;

        Float operator+(const Float& other) const;
        Float operator-(const Float& other) const;
    };
}

#endif