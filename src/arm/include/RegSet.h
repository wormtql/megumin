//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_REGSET_H
#define MEGUMIN_REGSET_H

#include <random>
#include <iostream>
#include "RegBank.h"

namespace arm {
    class Reg;

    class RegSet {
    private:
        bits gp{32, 0};
        bits fp{32, 0};
    public:
        RegSet() = default;

        [[nodiscard]] bool have_gp(int index) const;
        bool have_fp(int index) const;
        void set_fp(int index, bool value);
        void set_gp(int index, bool value);
        bool have_reg(const Reg& reg) const;
        void add_reg(const Reg& other);

        RegSet operator|(const Reg& other);

        int random_gp(std::mt19937& generator) const;
        int random_fp(std::mt19937& generator) const;
    };

    enum class RegType {
        None,
        GP,
        FP,
    };

    class Reg {
    public:
        RegType reg_type;
        int index;
    public:
        Reg();
        Reg(RegType reg_type, int index);

        static Reg fp(int index);
        static Reg gp(int index);
    };
}

std::ostream& operator<<(std::ostream& os, const arm::RegSet& reg_set);

#endif //MEGUMIN_REGSET_H
