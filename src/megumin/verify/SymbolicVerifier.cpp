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
    SymbolicVerifier::SymbolicVerifier(std::ostream &os): error_output_stream(os) {}

    VerifyResult megumin::SymbolicVerifier::verify(const arm::Program &target, const arm::Program &rewrite) {
        context c;

        arm::MachineStateS state1{c, "s"};

        arm::MachineStateS state2{state1};
        arm::MachineStateS original_state{state1};

        target.execute(state1);
        rewrite.execute(state2);

//        cout << state2.p_state.c << endl;

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

//                auto temp1 = state2.get_gp(64, 26, false, true);
//                cout << model.eval(temp1) << endl;
//                cout << model.eval(z3::shl(temp1, 52)) << endl;
//                cout << model.eval(~z3::shl(temp1, 52)) << endl;
//                auto temp3 = ~z3::shl(temp1, 52);
//                auto g27 = state2.get_gp(64, 27, false, true);
//                cout << model.eval(g27) << endl;
//                cout << model.eval()

                auto print_error_message = [&] () {
                    cout << "==== impossible" << endl;
                    if (test_state1 == test_state2) {
                        cout << "reason: The SMT found a counter example, but the symbolic state are the same" << endl;
                    } else if (test_state1 != symbolic_state1) {
                        cout << "reason: Machine state1 does not match symbolic state1" << endl;
                    } else if (test_state2 != symbolic_state2) {
                        cout << "reason: Machine state2 does not match symbolic state2" << endl;
                    }

                    error_output_stream << "counter example:" << endl;
                    error_output_stream << counter_example << endl;
                    error_output_stream << "test state 1:" << endl;
                    error_output_stream << test_state1 << endl;
                    error_output_stream << "test state 2:" << endl;
                    error_output_stream << test_state2 << endl;
                    error_output_stream << "symbolic state 1:" << endl;
                    error_output_stream << symbolic_state1 << endl;
                    error_output_stream << "symbolic state 2:" << endl;
                    error_output_stream << symbolic_state2 << endl;
                    error_output_stream << "target program:" << endl;
                    error_output_stream << target << endl;
                    error_output_stream << "rewrite program:" << endl;
                    error_output_stream << rewrite << endl;
                };

                bool x = false;
                assert(test_state1 == symbolic_state1);
                assert(test_state2 == symbolic_state2);
                if (test_state1 == test_state2 || test_state1 != symbolic_state1 || test_state2 != symbolic_state2) {
                    print_error_message();
                    assert(false);
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

