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
    auto target = megumin::aarch64_asm(R"(lsr x13, x11, #3
subs w8, w8, #0
csinc w0, w31, w31, lt
subs x8, x12, #1
madd x18, x8, x16, x31
mov x8, x31
and w0, w0, #1
ands w0, w0, #1
csel x18, x18, x8, ne
add x10, x10, x18
subs x16, x16, #1
and w17, w17, #1
ands w17, w17, #1
csel x16, x16, x8, ne
add x10, x10, x16
subs x11, x11, #8
csinc w11, w31, w31, cs
)").value();
    auto rewrite = megumin::aarch64_asm(R"(lsr x13, x11, #3
csneg w11, w31, w16, le
nop
nop
add x16, x16, x16, asr #48
smulh x10, x16, x10
nop
nop
nop
nop
nop
nop
cls w8, w12
rbit x0, x8
nop
udiv x17, x13, x1
lslv w18, w0, w10)").value();

    MachineState counter_example{};
    counter_example.gp.set_raw_i64(1, 1);
    counter_example.gp.set_raw_i64(10, -1613671628938316361);
    counter_example.gp.set_raw_i64(12, -5604949622140432320);
    counter_example.gp.set_raw_i64(16, -3691816049807715643);
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