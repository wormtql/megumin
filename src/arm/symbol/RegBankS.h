//
// Created by 58413 on 2023/6/24.
//

#ifndef MEGUMIN_REGBANKS_H
#define MEGUMIN_REGBANKS_H

#include <z3++.h>
#include <string>
#include <vector>

using std::string;
using namespace z3;

namespace arm {
    class GPRegBankS {
    private:
        std::vector<expr> bank;
        context ctx;
    public:
        GPRegBankS(z3::context& context, const string& prefix);
        GPRegBankS(const GPRegBankS& other);

        void set(int index, expr value);
        [[nodiscard]] expr get32(int index) const;
        [[nodiscard]] expr get64(int index) const;
        [[nodiscard]] expr get(int size, int index) const;

        expr operator==(const GPRegBankS& other) const;
        expr operator!=(const GPRegBankS&& other) const;
    };
}

class RegBankS {

};


#endif //MEGUMIN_REGBANKS_H
