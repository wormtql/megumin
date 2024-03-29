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
    auto target = megumin::aarch64_asm(R"(subs x9, x12, x14
csinc w13, w31, w31, le
subs x9, x12, x14
subs x15, x12, x14
csinc w15, w31, w31, ge
and w15, w15, #1
ands w15, w15, #1
csel x2, x12, x14, ne
mov x12, x31
and w13, w13, #1
ands w13, w13, #1
csel x9, x9, x12, ne
)").value();
    auto rewrite = megumin::aarch64_asm(R"(csel x2, x12, x14, le
nop
nop
nop
cls x15, x2
nop
nop
sub x9, x12, x2
nop
ands w13, w12, w14, lsr #31
sdiv w12, w9, w1
nop)").value();

    MachineState counter_example{};
    counter_example.gp.set_raw_i64(1, 4294967295);
    counter_example.gp.set_raw_i64(12, -8633209815772166145);
    counter_example.gp.set_raw_i64(14, -1873567812463884289);
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