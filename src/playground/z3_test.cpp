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
    return megumin::aarch64_asm("add x1, x2, x2; add x1, x1, x1").value();
//    return megumin::aarch64_asm("orr w1, w2, w1").value();
//    return megumin::aarch64_asm("lslv x1, x2, x3").value();
//    return megumin::aarch64_asm("ror x1, x2, #5").value();
//    return megumin::aarch64_asm("lsl x1, x1, #1").value();
//    return megumin::aarch64_asm("asr x1, x2, #1").value();
//    return megumin::aarch64_asm("add x1, x2, #10").value();
//    return megumin::aarch64_asm("movk x1, #10").value();
}

arm::Program get_prog2() {
    return megumin::aarch64_asm("lsl x1, x2, #2").value();
//    return megumin::aarch64_asm("ror x1, x2, #5").value();
//    return megumin::aarch64_asm("add x1, x1, #1").value();
//    return megumin::aarch64_asm("lsr x1, x2, #1").value();
//    return megumin::aarch64_asm("movk w1, #10").value();
//    return megumin::aarch64_asm("add x1, x2, #2; add x1, x1, #8").value();
//    return megumin::aarch64_asm("add x1, x2, #5").value();
}

int main() {
    context c;

    MachineStateS state1{c, "s"};
    MachineStateS state2{state1};

    auto prog1 = get_prog1();
    prog1.execute(state1);

    auto prog2 = get_prog2();
    prog2.execute(state2);

    expr e = state1 != state2;

//    auto x = c.bv_const("bv", 64);

//    expr x = c.bool_const("x");
//    expr y = c.bool_const("y");
//    expr conjecture = (!(x && y)) == (!x || !y);

    solver s(c);
//    s.add(!conjecture);
    s.add(e);
//    s.add(!((x + 10) == (x + 5)));

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