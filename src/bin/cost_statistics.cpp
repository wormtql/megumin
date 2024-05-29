#include <cstdint>
#include <iostream>
#include <vector>
#include <climits>
#include <random>
#include <fstream>

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

vector<double> f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{10000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
//    search.set_max_iteration(100);
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
//    for (int cost: state.cost_over_iterations) {
//        cout << "cost: " << cost << endl;
//    }

    return std::move(state.cost_over_iterations);
}

int main() {
    auto target = megumin::aarch64_asm(R"(movz w8, #135
and w9, w1, #1
tst w1, w8
csel w8, w31, w9, eq
cmp w1, #0
csinc w0, w8, w31, ne
)").value();

    cout << "Target program:" << endl;
    target.print();
    cout << endl;

    std::vector<MachineState> test_cases;
    for (int i = 0; i < 100; i++) {
        test_cases.emplace_back(MachineState{});
        test_cases[i].fill_gp_random();
        test_cases[i].fill_fp_random();
        test_cases[i].fill_nzcv_random();
        test_cases[i].fill_sp_random();
    }

    auto start = std::chrono::steady_clock::now();
    auto cost_over_iterations = f(target, test_cases, 0);
    auto now = std::chrono::steady_clock::now();
    auto duration = now - start;
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    cout << "duration: " << duration << endl;

    ofstream output_file{"cost_over_iterations.txt", ios::out};
    for (auto cost: cost_over_iterations) {
        output_file << cost << endl;
    }
}