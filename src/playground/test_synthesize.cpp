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

using namespace arm;
using std::cout;
using std::endl;
using std::vector;

void f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{1000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};
    weighted_program_mutation.set_use_integral_instructions(false);

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    megumin::SearchState state;
    search.set_max_time(6000000);

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
    auto program = megumin::aarch64_asm(R"(fmov d1, d2; fadd d1, d1, d3)").value();



    cout << "size: " << program.get_size() << endl;
    program.print();
    cout << endl;

    std::vector<MachineState> test_cases;
    for (int i = 0; i < 100; i++) {
        test_cases.emplace_back(MachineState{});
        test_cases[i].fill_gp_random();
        test_cases[i].fill_fp_random();
        test_cases[i].fill_nzcv_random();
        test_cases[i].fill_sp_random();
    }

//    program.print();
    f(program, test_cases, 1);

//    megumin::CorrectnessCost cost{program, std::move(test_cases)};
//
//    Program rewrite;
//    rewrite.add_instruction(instruction);
////    rewrite.add_instruction(instruction2);
//
//    auto c = cost.cost(rewrite, DBL_MAX);
//    cout << "cost: " << c.second << endl;
}