#include <cassert>
#include "MyFloat.h"


namespace arm {
    Float::Float(const bits& bs) {
        int size = bs.size;
        assert(size == 64 || size == 32 || size == 16);
        
        if (size == 64) {
            this->type = 0;
            this->value.double_value = bs.as_f64();
        } else if (size == 32) {
            this->type = 1;
            this->value.float_value = bs.as_f32();
        } else if (size == 16) {
            assert(false); // future work
        }
    }

    Float::Float(int size) {
        assert(size == 64 || size == 32 || size == 16);
        if (size == 64) {
            this->type = 0;
        } else if (size == 32) {
            this->type = 1;
        } else if (size == 16) {
            this->type = 2;
            assert(false);
        }

        this->value.double_value = 0.0;
    }

    Float::Float(double value) {
        this->type = 0;
        this->value.double_value = value;
    }

    Float::Float(float value) {
        this->type = 1;
        this->value.float_value = value;
    }

    Float Float::operator+(const Float& other) const {
        assert(type == other.type);

        if (type == 0) {
            double v = value.double_value + other.value.double_value;
            return v;
        } else if (type == 1) {
            float v = value.float_value + other.value.float_value;
            return v;
        }
        assert(false);
    }

    Float Float::operator-(const Float& other) const {
        assert(type == other.type);

        if (type == 0) {
            double v = value.double_value - other.value.double_value;
            return v;
        } else if (type == 1) {
            float v = value.float_value - other.value.float_value;
            return v;
        }
        assert(false);
    }

    bits Float::to_bits() const {
        if (type == 0) {
            return bits{value.double_value};
        } else if (type == 1) {
            return bits{value.float_value};
        }
        assert(false);
    }


}