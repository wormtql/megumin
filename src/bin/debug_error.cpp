#include <cstdint>
#include <iostream>
#include <vector>
#include <climits>
#include <random>

#include <z3++.h>

#include <Instruction.h>
#include <Program.h>
#include <cost/CorrectnessCost.h>
#include <cost/SimpleCost.h>
#include <search/Search.h>
#include <program_mutation/WeightedProgramMutation.h>
#include <random_instruction/RandomDataProcessingImm.h>
#include <blackbox_program/blackbox_program.h>
#include <megumin_utils.h>
#include <verify/SymbolicVerifier.h>

using namespace arm;
using namespace megumin;
using namespace std;

int main() {
    auto target = megumin::aarch64_asm(R"(extr x0, x16, x14, #32
extr x16, x17, x16, #32
subs x13, x13, x0
csinc w0, w31, w31, cs
sub x0, x15, x0
subs x31, x0, x15
csinc w15, w31, w31, ls
subs x16, x0, x16
)").value();
    auto rewrite = megumin::aarch64_asm(R"(extr x0, x16, x14, #32
extr x16, x17, x16, #32
subs x13, x13, x0
sbc x0, x15, x31
nop
nop
subs x16, x0, x16
sdiv x15, x0, x16
)").value();

    MachineState counter_example{};
    counter_example.gp.set_raw_i64(13, -2);
    counter_example.gp.set_raw_i64(15, -9223372036854775808);
    counter_example.gp.set_raw_i64(16, 4294967296);
    counter_example.gp.set_raw_i64(17, 2147483648);
//    counter_example.gp.set_raw_i64(13, -17979214137393153);
    MachineState actual_target_state{counter_example};
    MachineState actual_rewrite_state{counter_example};
    target.execute(actual_target_state);
    rewrite.execute(actual_rewrite_state);

    context c;
    model m(c);
    MachineStateS counter_example_symbolic_state = MachineStateS::from_machine_state_literal(c, counter_example);
    MachineStateS target_symbolic_state{counter_example_symbolic_state};
    MachineStateS rewrite_symbolic_state{counter_example_symbolic_state};
    target.execute(target_symbolic_state);
    rewrite.execute(rewrite_symbolic_state);

    MachineState target_symbolic_eval = target_symbolic_state.to_machine_state(m);
    MachineState rewrite_symbolic_eval = rewrite_symbolic_state.to_machine_state(m);

    assert(actual_target_state != actual_rewrite_state);
    assert(target_symbolic_eval != rewrite_symbolic_eval);
    assert(actual_target_state == target_symbolic_eval);
    assert(actual_rewrite_state == rewrite_symbolic_eval);

//    auto x = target_symbolic_state.to_machine_state(m);
//    for (int i = 0; i < 31; i++) {
////        cout << target_symbolic_state.gp.get(64, i).get_numeral_int64() << endl;
//        cout << x.gp.get_ref(i).data0 << endl;
//    }

//
//    SymbolicVerifier verifier{};
//    VerifyResult result = verifier.verify(target, rewrite);
//
//    cout << result.success << endl;
}