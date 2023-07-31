//
// Created by 58413 on 2023/6/19.
//

#include <iostream>
#include <bitset>
#include <map>
#include <random>
#include <fstream>
#include <optional>

#include <MachineState.h>
#include <Bitvec.h>
#include <Instruction.h>
#include <bb/BasicBlock.h>
#include <bb/BBExtractor.h>
#include <visitor/CanHandleThisInst.h>
#include <Program.h>
#include <cost/SimpleCost.h>
#include <program_mutation/WeightedProgramMutation.h>
#include <search/Search.h>
#include <verify/BruteForceVerifier.h>
#include <verify/SymbolicVerifier.h>

using namespace std;
using namespace arm;

bool can_process(const BasicBlock& bb) {
    CanHandleThisInst can;
    for (const auto& inst: bb.get_instructions()) {
        can.result = false;
        can.visit_instruction(inst);

        if (!can.result) {
            return false;
        }
    }
    return true;
}

//ofstream correct_file{"correct.txt"};
ofstream correct_file{R"(E:\CLionProjects\megumin\correct.txt)"};

std::optional<arm::Program> f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{10000};
    cout << "[optimization target]" << endl;
    target.print();
    cout << endl;

//    megumin::BruteForceVerifier verifier{100000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    search.set_max_time(10000);
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

    if (state.success) {
//        auto verify_result = verifier.verify(target, state.current_correct_best);
//        if (verify_result.success) {
//            correct_file << "[optimization success]" << endl;
//            target.print(correct_file);
//            correct_file << endl << ">>>" << endl;
//            state.current_correct_best.print(correct_file);
//            correct_file << "\n\n";
//            correct_file.flush();
//
//            cout << "[optimization success]" << endl;
//            target.print(cout);
//            cout << endl << ">>>" << endl;
//            state.current_correct_best.print(cout);
//            cout << "\n\n";
//        }

        return state.current_correct_best;
    }

    return {};
}

int main() {
    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\pocketfft-aarch64.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\raytracinginoneweekend.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\rt2.s)"};
//    extractor.set_max_bb(-1);
    extractor.set_max_bb(-1);
    auto bbs = extractor.extract_basic_blocks();

    vector<BasicBlock> viable_bbs;
    for (const auto& bb: bbs) {
        if (can_process(bb)) {
            viable_bbs.push_back(bb);
        }
    }

    cout << "viable basic block count: " << viable_bbs.size() << endl;

    for (int i = 0; i < viable_bbs.size(); i++) {
        bool success = false;
        auto prog = viable_bbs[i].to_program();
        cout << viable_bbs[i];

//        if (viable_bbs[i].get_start() <54112) {
//            continue;
//        }

        std::vector<MachineState> test_cases;
        for (int ii = 0; ii < 1; ii++) {
            test_cases.emplace_back(MachineState{});
            test_cases[ii].fill_gp_random();
            test_cases[ii].fill_fp_random();
            test_cases[ii].fill_nzcv_random();
            test_cases[ii].fill_sp_random();
        }

        while (!success && test_cases.size() <= 200) {
            auto result = f(prog, test_cases, 1);
            if (result.has_value()) {
                // find a solution
                auto rewrite = result.value();
                auto verifier = megumin::SymbolicVerifier();
                auto verify_result = verifier.verify(prog, rewrite);
                if (verify_result.success) {
                    success = true;

                    correct_file << "[optimization success]" << endl;
                    correct_file << viable_bbs[i].get_start() << ", " << viable_bbs[i].get_end() << endl;
                    prog.print(correct_file);
                    correct_file << endl << ">>>" << endl;
                    rewrite.print(correct_file);
                    correct_file << "\n\n";
                    correct_file.flush();

                    cout << "[optimization success]" << endl;
                    prog.print(cout);
                    cout << endl << ">>>" << endl;
                    rewrite.print(cout);
                    cout << "\n\n";
                } else {
                    if (verify_result.counter_example.has_value()) {
                        cout << "found counter example:" << endl;
//                        cout << verify_result.counter_example.value()
                        test_cases.push_back(verify_result.counter_example.value());
                    }
                }
            } else {
                // did not find a solution
                break;
            }
        }
    }

//    std::vector<MachineState> test_cases;
//    for (int i = 0; i < 100; i++) {
//        test_cases.emplace_back(MachineState{});
//        test_cases[i].fill_gp_random();
//        test_cases[i].fill_fp_random();
//        test_cases[i].fill_nzcv_random();
//        test_cases[i].fill_sp_random();
//    }
//
//    for (int i = 0; i < viable_bbs.size(); i++) {
//        const auto& bb = viable_bbs[i];
//        auto prog = bb.to_program();
//
//        f(prog, test_cases, 1);
//    }


    return 0;
}