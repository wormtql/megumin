#include <cstdint>
#include <iostream>
#include <vector>
#include <climits>
#include <random>

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

void f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{10000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    if (target.is_all_integral_instructions()) {
        search.set_use_fp(false);
        search.set_use_integral(true);
    } else if (target.is_all_fp_instructions()) {
        search.set_use_fp(true);
        search.set_use_integral(false);
    } else {
        search.set_use_integral(true);
        search.set_use_fp(true);
    }
    megumin::SearchState state;

    arm::Program init_program{};

    if (init_mode == 0) {
        arm::Program empty_program{};
        for (int i = 0; i < target.get_size(); i++) {
            empty_program.add_instruction(Instruction::nop());
        }
        init_program = empty_program;
    } else if (init_mode == 1) {
        init_program = target;
    }


    RegSet min_def_ins = target.get_minimum_def_ins();
    min_def_ins.set_fp(1, true);
    min_def_ins.set_gp(1, true);
    cout << min_def_ins << endl;

    init_program.set_entry_def_ins(min_def_ins);
    init_program.calculate_def_ins();

    state.current = init_program;
    state.current_cost = simple_cost.cost(init_program, std::numeric_limits<double>::max()).second;
    cout << state.current_cost << endl;
    state.current_correct_best = target;
    state.current_correct_best_cost = state.current_cost;
    state.current_best = init_program;
    state.current_best_cost = state.current_cost;

    search.do_search(state);
}

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

    SymbolicVerifier verifier{};
    VerifyResult result = verifier.verify(target, rewrite);

    cout << result.success << endl;

//    program.print();
//    cout << endl;
//
//    std::vector<MachineState> test_cases;
//    for (int i = 0; i < 100; i++) {
//        test_cases.emplace_back(MachineState{});
//        test_cases[i].fill_gp_random();
//        test_cases[i].fill_fp_random();
//        test_cases[i].fill_nzcv_random();
//        test_cases[i].fill_sp_random();
//    }
//
//    f(program, test_cases, 0);
}