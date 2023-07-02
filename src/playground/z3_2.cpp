//
// Created by 58413 on 2023/6/24.
//

#include <iostream>
#include <z3++.h>

#include "symbol/InstructionExecutionS.h"
#include "Program.h"
#include "megumin_utils.h"

using namespace z3;
using namespace arm;
using namespace std;

arm::Program get_prog1() {
    return megumin::aarch64_asm("rorv x1, x2, x4").value();
//    return megumin::aarch64_asm("ror x1, x2, #5").value();
//    return megumin::aarch64_asm("lsl x1, x1, #1").value();
//    return megumin::aarch64_asm("asr x1, x2, #1").value();
//    return megumin::aarch64_asm("add x1, x2, #10").value();
//    return megumin::aarch64_asm("movk x1, #10").value();
}

arm::Program get_prog2() {
    return megumin::aarch64_asm("rorv x1, x2, x4").value();
//    return megumin::aarch64_asm("ror x1, x2, #5").value();
//    return megumin::aarch64_asm("add x1, x1, #1").value();
//    return megumin::aarch64_asm("lsr x1, x2, #1").value();
//    return megumin::aarch64_asm("movk w1, #10").value();
//    return megumin::aarch64_asm("add x1, x2, #2; add x1, x1, #8").value();
//    return megumin::aarch64_asm("add x1, x2, #5").value();
}

int main() {
    context c;

    expr x = c.bool_val(true);
    x = shl(x, 1);

//    expr x = c.bv_val(4, 64);
//    expr x = c.bv_const("x", 64);
    expr y = c.bv_val(-63, 64);
//    expr y = c.bv_val(55, 64);
//    expr z = z3::shl(x, y % 64);
    expr z = c.bv_const("z", 64);

//    cout << z.as_uint64();

    solver s(c);
    s.add(z == z3::shl(x, y));

    std::cout << s << "\n";
//    std::cout << s.to_smt2() << "\n";

    switch (s.check()) {
        case unsat: {
            cout << "unsat" << endl;

            break;
        }
        case sat: {
            cout << "sat" << endl;
            auto model = s.get_model();
//            std::cout << s.get_model();
            cout << model.size() << endl;

            for (int i = 0; i < model.size(); i++) {
                func_decl v = model[i];
                // this problem contains only constants
                assert(v.arity() == 0);
                std::cout << v.name() << " = " << model.get_const_interp(v) << "\n";
            }

            break;
        }
        case unknown: std::cout << "unknown\n"; break;
    }
    cout << std::flush;

    return 0;
}