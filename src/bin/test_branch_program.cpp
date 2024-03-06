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

arm::Program get_branch_program() {
    Program program{};

    std::vector<arm::Instruction> bb0 = megumin::aarch64_asm_basic_block("cmp w0, 9\nbgt 0").value();
    bb0[1].set_branch_target(2);
    std::vector<arm::Instruction> bb1 = megumin::aarch64_asm_basic_block("mov w0, 0\nb 0").value();
    bb1[1].set_branch_target(3);
    std::vector<arm::Instruction> bb2 = megumin::aarch64_asm_basic_block("mov w0, 1").value();

    program.add_basic_block(bb0, {{1, 2}});
    program.add_basic_block(bb1, {3});
    program.add_basic_block(bb2, {3});
    program.add_nop_basic_block();
    program.calc_in_connections_from_out_connections();

    program.calculate_def_ins();

    return program;
}

void f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{10000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    if (target.is_all_integral_instructions_except_branch()) {
        search.set_use_fp(false);
        search.set_use_integral(true);
    } else if (target.is_all_fp_instructions_except_branch()) {
        search.set_use_fp(true);
        search.set_use_integral(false);
    } else {
        search.set_use_integral(true);
        search.set_use_fp(true);
    }
    megumin::SearchState state;

    arm::Program init_program{};

    if (init_mode == 0) {
        init_program = target.clone_program_all_nop();
    } else if (init_mode == 1) {
        init_program = target;
    }


    RegSet min_def_ins = target.get_minimum_def_ins();
    if (!min_def_ins.has_any_fp()) {
        min_def_ins.set_fp(1, true);
    }
    if (!min_def_ins.has_any_gp()) {
        min_def_ins.set_gp(1, true);
    }
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
    auto program = get_branch_program();

    cout << "size: " << program.get_size() << endl;
    program.print_with_def_ins();
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