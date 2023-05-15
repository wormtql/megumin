//
// Created by 58413 on 2023/4/14.
//

#include <cassert>
#include <initializer_list>
#include <utility>

#include "Bitvec.h"

inline int64_t get_mask(int size) {
    if (size == 64) {
        return ~0ll;
    } else {
        return (1ll << size) - 1;
    }
}

namespace arm {
    bits bits::from_bools(std::initializer_list<bool> args) {
        assert(args.size() <= 64);
        int64_t result = 0;
        for (bool i: args) {
            result = (result << 1) | (int)i;
        }
        return bits{(int)args.size(), result};
    }

    bits bits::ones(int size) {
        return bits{size, (1 << size) - 1};
    }

    bits bits::concat(const bits& op) {
//        assert(size + op.size <= 64);
        int64_t result = data0;
        result = (result << op.size) | (op.data0 & get_mask(op.size));
        return bits{size + op.size, result};
    }

    bits::bits(int size, int64_t init): size(size) {
        data0 = init & get_mask(size);
    }

    bits::bits(int size): size(size), data0(0) {}

    bits::bits(double f) {
        size = 64;
        const auto* temp = reinterpret_cast<const int64_t*>(&f);
        data0 = *temp;
    }

    bits::bits(float f) {
        size = 32;
        const auto* temp = reinterpret_cast<const int32_t*>(&f);
        int32_t i32 = *temp;
        data0 = i32;
    }

    bool bits::is_set(int index) const {
        assert(index < size);
        return (data0 >> index) & 1ll;
    }

    bits bits::get_range(int low, int high) const {
        assert(low < size && high <= size);
        int64_t temp = (data0) & get_mask(high);
        temp = temp >> low;
        return bits{high - low, temp};
    }

    int32_t bits::as_i32() const {
        auto mask = ((1ll << 32) - 1);
        return static_cast<int32_t>(mask & data0);
    }

    // this will not sign extend to value
    int64_t bits::as_i64() const {
        return data0;
    }

    uint32_t bits::as_u32() const {
        auto mask = ((1ll << 32) - 1);
        return static_cast<uint32_t>(mask & data0);
    }

    uint64_t bits::as_u64() const {
        return data0 & get_mask(size);
    }

    bool bits::operator==(const bits& other) const {
        assert(size == other.size);
        return data0 == other.data0;
    }

    bool bits::operator!=(const bits& other) const {
        assert(size == other.size);
        return data0 != other.data0;
    }

    bool bits::operator==(int64_t other) const {
        int64_t mask = get_mask(size);
        return data0 == (other & mask);
    }

    bool bits::operator!=(int64_t other) const {
        int64_t mask = get_mask(size);
        return data0 != (other & mask);
    }

    bits bits::operator>>(int size) const {
        int64_t result = data0 >> size;
        return bits{this->size, result};
    }

    bits bits::operator<<(int size) const {
        int64_t result = data0 << size;
        return bits{this->size, result};
    }

    bits bits::operator&(int64_t other) const {
        int64_t result = data0 & other;
        return bits{this->size,  result};
    }

    bits bits::operator&(const bits& other) const {
        assert(size == other.size);
        return bits{ size, data0 & other.data0 };
    }

    bits bits::operator|(const bits &other) const {
        assert(size == other.size);
        return bits{size, data0 | other.data0};
    }

    bits bits::operator^(const bits &other) const {
        assert(size == other.size);
        return bits{size, data0 ^ other.data0};
    }

    bits bits::operator+(int64_t other) const {
        int64_t result = data0 + other;
        result &= get_mask(size);
        return bits{size,  result};
    }

    bits bits::operator~() const {
        int64_t result = ~data0;
        result &= get_mask(size);
        return bits{size, result};
    }

    bits& bits::append_bit(bool bit) {
        int b = bit;
        data0 = (data0 << 1) | b;
        size += 1;
        return *this;
    }

    bits bits::zero_extend(int size) const {
        assert(size >= this->size);
        return bits{size, data0};
    }

    bits bits::sign_extend(int size) const {
        assert(size >= this->size);
        bool sign = is_set(this->size - 1);

        bits result;
        if (sign)
            result = ones(size);
        else
            result = bits{size, 0};
        result.set_range(0, this->size, data0);
        return result;
    }

    bits bits::resize(int size) const {
        int64_t result = data0 & get_mask(size);
        return bits{size, result};
    }

    void bits::set_value(int64_t value) {
        data0 = value & get_mask(size);
    }

    void bits::set_value(const bits &value) {
        assert(size == value.size);
        data0 = value.data0;
    }

    bits& bits::set_bit(int index, bool value) {
        assert(index < size);
        if (value) {
            data0 |= 1ll << index;
        } else {
            data0 &= ~(1ll << index);
        }
        return *this;
    }

    bits& bits::set_range(int low, int high, int64_t value) {
        assert(low < size && high <= size);
//        int64_t mask = ~0ll;
//        mask <<= high;
//        mask |= get_mask(low);
        int64_t mask = get_mask(high) - get_mask(low);
        int64_t maskr = ~mask;
        data0 &= maskr;
//        value <<= low;
//        value &= ~mask;
//        value &= mask;
        value <<= low;
        value &= mask;
        data0 |= value;
        return *this;
    }

    int bits::operator[](int index) const {
        return is_set(index);
    }

    bits bits::operator[](std::pair<int, int> range) const {
        return get_range(range.first, range.second);
    }

    void bits::clear() {
        this->data0 = 0;
    }

    double bits::as_f64() const {
        const double* temp = reinterpret_cast<const double*>(&data0);
        return *temp;
    }

    float bits::as_f32() const {
        int32_t i32 = static_cast<int32_t>(data0);
        const float* temp = reinterpret_cast<const float*>(&i32);
        return *temp;
    }
}
