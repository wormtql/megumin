//
// Created by 58413 on 2023/7/3.
//

#include <z3++.h>
#include <iostream>
#include "SymbolicVerifier.h"
#include <symbol/MachineStateS.h>

using namespace z3;
using std::cout;
using std::endl;

namespace megumin {
    VerifyResult megumin::SymbolicVerifier::verify(const arm::Program &target, const arm::Program &rewrite) {
        context c;

        arm::MachineStateS state1{c, "s"};

        arm::MachineStateS state2{state1};
        arm::MachineStateS original_state{state1};

        target.execute(state1);
        rewrite.execute(state2);

        expr e = state1 != state2;

        solver s(c);
        s.add(e);
        cout << s << endl << std::flush;

        switch (s.check()) {
            case unsat: {
                // success
                return VerifyResult{true, {}};
            }
            case sat: {
                cout << "sat" << endl;
                auto model = s.get_model();

                int temp = model.eval(c.bv_val(0, 64), false);
                for (int i = 0; i < model.size(); i++) {
                    func_decl v = model[i];
                    // this problem contains only constants
                    assert(v.arity() == 0);
                    std::cout << v.name() << " = " << model.get_const_interp(v) << "\n";
                }

                arm::MachineState counter_example = original_state.to_machine_state(model);
                counter_example.gp.get_mut_ref(31) = arm::bits{64, 0};

                arm::MachineState test_state1{counter_example};
                arm::MachineState test_state2{counter_example};
                target.execute(test_state1);
                rewrite.execute(test_state2);

                arm::MachineState symbolic_state1 = state1.to_machine_state(model);
                arm::MachineState symbolic_state2 = state2.to_machine_state(model);

                bool x = false;
                if (test_state1 == test_state2) {
                    cout << "impossible" << endl;
                    x = symbolic_state1 == symbolic_state2;
                    bool y = model.eval(e);
                    cout << x << endl << std::flush;


                    for (int i = 0; i < target.get_size(); i++) {
                        arm::MachineStateS symbolic_s{original_state};
                        arm::MachineState machine_s{counter_example};

                        for (int j = 0; j <= i; j++) {
                            target.get_instruction_const(j).execute(symbolic_s);
                            target.get_instruction_const(j).execute(machine_s);
                        }
                        cout << symbolic_s.p_state.n << endl;
                        cout << symbolic_s.p_state.z << endl;
                        cout << symbolic_s.p_state.c << endl;
                        cout << symbolic_s.p_state.v << endl;

                        arm::MachineState ss = symbolic_s.to_machine_state(model);
                        bool xx = ss == machine_s;
                    }
                }




                return VerifyResult{false, counter_example};
            }
            case unknown: {
                return VerifyResult{false, {}};
            }
        }
    }
}

