//
// Created by 58413 on 2023/7/3.
//

#include <z3++.h>
#include <iostream>
#include "SymbolicVerifier.h"
#include <symbol/MachineStateS.h>
#include "megumin_utils.h"

using namespace z3;
using std::cout;
using std::endl;

namespace megumin {
    VerifyResult megumin::SymbolicVerifier::verify(const arm::Program &target, const arm::Program &rewrite) {
        context c;

        /// symbolic state after execution of target
        arm::MachineStateS state1{c, "s"};
        /// symbolic state after execution of rewrite
        arm::MachineStateS state2{state1};
        /// the state before any execution
        arm::MachineStateS original_state{state1};

        target.execute(state1);
        rewrite.execute(state2);

        expr e = state1 != state2;

        solver s(c);
        s.add(e);

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

                /// counter example, before any execution
                arm::MachineState counter_example = original_state.to_machine_state(model);
                /// the x31 must be 0
                counter_example.gp.get_mut_ref(31) = arm::bits{64, 0};

                /// counter example after execution by target
                arm::MachineState test_state1{counter_example};
                /// counter example after execution by rewrite
                /// since it's a counter example, we expect test_state1 and test_state2 are not same
                arm::MachineState test_state2{counter_example};
                target.execute(test_state1);
                rewrite.execute(test_state2);

                /// counter example after execution by symbolic execution of target
                arm::MachineState symbolic_state1 = state1.to_machine_state(model);
                /// counter example after execution by symbolic execution of rewrite
                arm::MachineState symbolic_state2 = state2.to_machine_state(model);

                auto setup_error_message = [&] () {
                    SymbolicVerifyDebugInfo debug_info;

                    cout << "==== impossible" << endl;
                    if (test_state1 == test_state2) {
                        debug_info.reason = "The SMT found a counter example, but the symbolic state are the same";
                    } else if (test_state1 != symbolic_state1) {
                        debug_info.reason = "Machine state1 does not match symbolic state1";
                    } else if (test_state2 != symbolic_state2) {
                        debug_info.reason = "Machine state2 does not match symbolic state2";
                    }

                    debug_info.counter_example = counter_example;
                    debug_info.test_state1 = test_state1;
                    debug_info.test_state2 = test_state2;
                    debug_info.symbolic_state1 = symbolic_state1;
                    debug_info.symbolic_state2 = symbolic_state2;
                    debug_info.target = target;
                    debug_info.rewrite = rewrite;

                    error_debug_info = debug_info;
                };

                bool x = false;
                /// if this assertion fails, the actual execution and symbolic execution differs, we have to check
//                assert(test_state1 == symbolic_state1);
//                assert(test_state2 == symbolic_state2);
                /// test_state1 == test_state2: the SMT found a counter example, but the actual execution end up with same results
                if (test_state1 == test_state2 || test_state1 != symbolic_state1 || test_state2 != symbolic_state2) {
                    setup_error_message();
//                    assert(false);
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

    ostream& operator<<(ostream& os, const SymbolicVerifier::SymbolicVerifyDebugInfo& debug_info) {
        os << "counter example:" << endl;
        os << "reason: " << debug_info.reason << endl;
        os << debug_info.counter_example << endl;
        os << "test state 1:" << endl;
        os << debug_info.test_state1 << endl;
        os << "test state 2:" << endl;
        os << debug_info.test_state2 << endl;
        os << "symbolic state 1:" << endl;
        os << debug_info.symbolic_state1 << endl;
        os << "symbolic state 2:" << endl;
        os << debug_info.symbolic_state2 << endl;
        os << "target program:" << endl;
        os << debug_info.target << endl;
        os << "rewrite program:" << endl;
        os << debug_info.rewrite << endl;

        return os;
    }
}

