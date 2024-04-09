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
    auto target = megumin::aarch64_asm(R"(add x10, x10, x11
add x11, x11, #1
sub x13, x9, x10
eor x12, x10, x9
eor x10, x13, x10
orr x10, x10, x12
sub x12, x5, x11
eor x10, x10, x9
bic x12, x12, x5
orr x10, x10, x12
orn x10, x31, x10
sbfm x7, x10, #63, #63
and x10, x11, x7
sub x9, x9, x10
)").value();
    auto rewrite = megumin::aarch64_asm(R"(add x10, x10, x11
sub x13, x9, x10
sdiv x12, x10, x13
add x11, x11, #1
eon x10, x12, x10, asr #62
sub x12, x5, x11
nop
bic x12, x12, x5
nop
eor x7, x31, x10, asr #63
and x10, x11, x7, ror #59
nop
sub x9, x9, x10
nop
)").value();

    MachineState counter_example{};
    counter_example.gp.set_raw_i64(5, 1099511627775);
    counter_example.gp.set_raw_i64(9, 9223372036854775807);
    counter_example.gp.set_raw_i64(10, -9223372036854775807);
    counter_example.gp.set_raw_i64(11, -1);
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