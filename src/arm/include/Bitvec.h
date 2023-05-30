//
// Created by 58413 on 2023/4/14.
//

#ifndef MEGUMIN_BITVEC_H
#define MEGUMIN_BITVEC_H

#include <cstdint>
#include <initializer_list>
#include <utility>

namespace arm {
    class bits {
    private:

    public:
        int64_t data0 = 0;
        int size = 0;

        bits(int size, int64_t init);
        explicit bits(double f);
        explicit bits(float f);
        explicit bits(int size);
        bits() = default;

        [[nodiscard]] bool is_set(int index) const;
        [[nodiscard]] bits get_range(int low, int high) const;
        void set_value(int64_t value);
        void set_value(const bits& value);

        [[nodiscard]] int32_t as_i32() const;
        [[nodiscard]] int64_t as_i64() const;
        [[nodiscard]] uint32_t as_u32() const;
        [[nodiscard]] uint64_t as_u64() const;
        [[nodiscard]] double as_f64() const;
        [[nodiscard]] float as_f32() const;

        bits& append_bit(bool bit);
        bits zero_extend(int size) const;
        bits resize(int size) const;
        [[nodiscard]] bits sign_extend(int size) const;
        void clear();
        void inverse_bit(int index);

        bool operator==(const bits& other) const;
        bool operator==(int64_t other) const;
        bool operator!=(const bits& other) const;
        bool operator!=(int64_t other) const;
        bits operator>>(int size) const;
        bits operator<<(int size) const;
        bits operator&(int64_t other) const;
        bits operator&(const bits& other) const;
        bits operator|(const bits& other) const;
        bits operator^(const bits& other) const;
        bits operator+(int64_t other) const;
        bits operator~() const;
        int operator[](int index) const;
        bits operator[](std::pair<int, int> range) const;

        bits concat(const bits& op);
        bits& set_bit(int index, bool value);
        bits& set_range(int low, int high, int64_t value);
        static bits ones(int size);
        static bits from_bools(std::initializer_list<bool> args);

        // helper functions for instructions
        [[nodiscard]] bits get_rd() const { return get_range(0, 5); }
        [[nodiscard]] bits get_rn() const { return get_range(5, 10); }
        [[nodiscard]] bits get_imms() const { return get_range(10, 16); }
    };
}

#endif //MEGUMIN_BITVEC_H
