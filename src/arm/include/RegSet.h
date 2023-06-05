//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_REGSET_H
#define MEGUMIN_REGSET_H

#include "RegBank.h"

namespace arm {
    class Reg;

    class RegSet {
    private:
        bits gp{32, 0};
        bits fp{32, 0};
    public:
        RegSet() = default;

        bool have_gp(int index);

        bool have_fp(int index);

        void set_fp(int index, bool value);

        void set_gp(int index, bool value);

        void add_reg(const Reg& other);

        RegSet operator|(const Reg& other);
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

#endif //MEGUMIN_REGSET_H
