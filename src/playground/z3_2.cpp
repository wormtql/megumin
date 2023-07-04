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

    // (= (bvadd s_gp_28 #xffffffffffffffff #x0000000000000001) #x0000000000000000)
    expr x = c.bv_val(1, 64);
    expr e = x + c.bv_val(0xffffffffffffffff, 64) + c.bv_val(1, 64);
    cout << x << endl;

    solver s(c);

    std::cout << s << "\n";

    switch (s.check()) {
        case unsat: {
            cout << "unsat" << endl;

            break;
        }
        case sat: {
            cout << "sat" << endl;
            auto model = s.get_model();
//            std::cout << s.get_model();

            for (int i = 0; i < model.size(); i++) {
                func_decl v = model[i];
                // this problem contains only constants
                assert(v.arity() == 0);
                std::cout << v.name() << " = " << model.get_const_interp(v) << "\n";
            }

            cout << model.eval(e == 0).bool_value() << endl;
            cout << model.eval(e == 0) << endl;

            break;
        }
        case unknown: std::cout << "unknown\n"; break;
    }
    cout << std::flush;

    return 0;
}