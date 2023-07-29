//
// Created by 58413 on 2023/6/24.
//

#include <megumin_utils.h>
#include <iostream>

#include <z3++.h>
#include <utility>
#include <sstream>
#include "RegBankS.h"

using namespace std;

namespace arm {
    GPRegBankS::GPRegBankS(z3::context &context, const string &prefix) {
        for (int i = 0; i < 31; i++) {
//            string ss;
            std::ostringstream oss;
            oss << prefix << "_" << i;

            bank.push_back(context.bv_const(oss.str().c_str(), 64));
        }
        bank.push_back(context.num_val(0, context.bv_sort(64)));
    }

    GPRegBankS::GPRegBankS(const GPRegBankS &other) {
        bank = other.bank;
    }

    void GPRegBankS::set(int index, expr value) {
        megumin::megumin_assert(index >= 0 && index < 32);
        if (index == 31) {
            return;
        }

        bank[index] = std::move(value);
    }

    expr GPRegBankS::get64(int index) const {
        return bank[index];
    }

    expr GPRegBankS::get32(int index) const {
        auto temp = bank[index];
        return temp.extract(31, 0);
    }

    expr GPRegBankS::get(int size, int index) const {
        if (size == 32) {
            return get32(index);
        } else if (size == 64) {
            return get64(index);
        } else {
            megumin::megumin_assert(false);
        }
    }

    z3::expr GPRegBankS::operator==(const GPRegBankS &other) const {
        expr result = bank[0] == other.bank[0];
        for (int i = 1; i < 31; i++) {
            result = result && bank[i] == other.bank[i];
        }
        return result;
    }

    z3::expr GPRegBankS::operator!=(const GPRegBankS &&other) const {
        return !(*this == other);
    }

    GPRegBank GPRegBankS::to_gp_reg_bank(const model &m) const {
        GPRegBank result;

        for (int i = 0; i < 32; i++) {
//            cout << this->bank[i].get_sort() << endl;
            int64_t x = (int64_t) m.eval(this->bank[i], true).get_numeral_uint64();
            bits b{64, x};
            result.get_mut_ref(i) = b;
        }

        return result;
    }
}
