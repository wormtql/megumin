//
// Created by 58413 on 2023/6/19.
//

#include <iostream>
#include <bitset>
#include <map>
#include <random>
#include <fstream>
#include <optional>
#include <thread>
#include <future>
#include <mutex>

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
#include <argparse/argparse.hpp>
#include <search/SearchResult.h>

using namespace std;
using namespace arm;
using namespace megumin;

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

std::optional<arm::Program> opt_once(const arm::Program& target, vector<MachineState> test_cases, int max_time, int init_mode = 1) {
    std::mt19937 generator{60000};
    cout << "[optimization target]" << endl;
    target.print();
    cout << endl;

//    megumin::BruteForceVerifier verifier{100000   };

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    search.set_max_time(max_time);
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

mutex g_error_stream;
mutex g_correct_stream;

void worker(promise<vector<SearchResult>> && p, const BasicBlock* bb_ptr, int bb_size, int time_per_opt, ostream* error_stream, ostream* correct_stream) {
    vector<SearchResult> results;

    for (int i = 0; i < bb_size; i++) {
        bool success = false;
        auto prog = bb_ptr[i].to_program();
        cout << bb_ptr[i];

        std::vector<MachineState> test_cases;
        int initial_test_cases_count = 1;
        // set initial test case
        for (int ii = 0; ii < initial_test_cases_count; ii++) {
            test_cases.emplace_back(MachineState{});
            test_cases[ii].fill_gp_random();
            test_cases[ii].fill_fp_random();
            test_cases[ii].fill_nzcv_random();
            test_cases[ii].fill_sp_random();
        }

        int max_test_case_count = 200;
        while (!success && test_cases.size() <= max_test_case_count) {
            int init_mode = 1;
            auto result = opt_once(prog, test_cases, time_per_opt, init_mode);
            if (result.has_value()) {
                // find a solution
                auto rewrite = result.value();
                auto verifier = megumin::SymbolicVerifier();
                auto verify_result = verifier.verify(prog, rewrite);

                // there is an error which is not expected
                if (verifier.error_debug_info.has_value()) {
                    lock_guard<mutex> guard(g_error_stream);
                    (*error_stream) << verifier.error_debug_info.value() << endl;
                }

                if (verify_result.success) {
                    success = true;

                    SearchResult search_result{ prog, rewrite, bb_ptr[i].get_start(), bb_ptr[i].get_end() };
                    results.push_back(search_result);

                    // write correct stream
                    {
                        lock_guard<mutex> guard(g_correct_stream);

                        ostream& cs = *correct_stream;

                        cs << "[optimization success]" << endl;
                        cs << bb_ptr[i].get_start() << ", " << bb_ptr[i].get_end() << endl;
                        prog.print(cs);
                        cs << endl << ">>>" << endl;
                        rewrite.print(cs);
                        cs << "\n\n";
                        cs.flush();
                    }

                    cout << "[optimization success]" << endl;
                    prog.print(cout);
                    cout << endl << ">>>" << endl;
                    rewrite.print(cout);
                    cout << "\n\n";
                } else {
                    if (verify_result.counter_example.has_value()) {
                        cout << "found counter example" << endl;
                        test_cases.push_back(verify_result.counter_example.value());
                    }
                }
            } else {
                // did not find a solution
                break;
            }
        }
    }

    p.set_value(move(results));
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("megumin auto opt");
    program.add_argument("--correct-file").default_value("correct.txt");
    program.add_argument("--input-file");
    program.add_argument("--error-file").default_value("error.txt");
    program.add_argument("--time-per-opt").default_value(10000).scan<'i', int>();
    program.add_argument("--thread-count").default_value(1).scan<'i', int>();

    program.parse_args(argc, argv);

    ofstream correct_file{program.get<std::string>("--correct-file")};
    BBExtractor extractor{program.get<string>("--input-file")};
    ofstream error_file{program.get<string>("--error-file")};

    if (!correct_file.is_open()) {
        cout << "cannot open " << program.get<string>("--correct-file") << endl;
    }
    if (!error_file.is_open()) {
        cout << "cannot open " << program.get<string>("--error-file") << endl;
    }

//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\pocketfft-aarch64.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\raytracinginoneweekend.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\rt2.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\rt3.s)"};
    // BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\crypto.s)"};
//    BBExtractor extractor{R"(/root/super/rt2.s)"};
//    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\a.s)"};
//    extractor.set_max_bb(-1);
    extractor.set_max_bb(-1);
    auto bbs = extractor.extract_basic_blocks();

    vector<BasicBlock> viable_bbs;
    for (const auto& bb: bbs) {
        if (can_process(bb)) {
            viable_bbs.push_back(bb);
        }
    }

    int viable_bb_size = viable_bbs.size();
    cout << "viable basic block count: " << viable_bb_size << endl;

    int thread_count = program.get<int>("--thread-count");
    int blocks_per_thread = viable_bb_size / thread_count;
    int last_block_count = viable_bb_size - blocks_per_thread * (thread_count - 1);

    int offset = 0;
    int time_per_opt = program.get<int>("--time-per-opt");

    vector<thread> threads;
    vector<future<vector<SearchResult>>> futures;

    for (int i = 0; i < thread_count; i++) {
        promise<vector<SearchResult>> prom;
        auto f = prom.get_future();

        int size = blocks_per_thread;
        if (i == thread_count - 1) {
            size = last_block_count;
        }

        thread t(&worker, move(prom), viable_bbs.data() + offset, size, time_per_opt, &error_file, &correct_file);

        futures.push_back(move(f));
        threads.push_back(move(t));
    }

    for (thread& t: threads) {
        t.join();
    }

//    for (int i = 0; i < viable_bbs.size(); i++) {
//        bool success = false;
//        auto prog = viable_bbs[i].to_program();
//        cout << viable_bbs[i];
//
//        std::vector<MachineState> test_cases;
//        for (int ii = 0; ii < 1; ii++) {
//            test_cases.emplace_back(MachineState{});
//            test_cases[ii].fill_gp_random();
//            test_cases[ii].fill_fp_random();
//            test_cases[ii].fill_nzcv_random();
//            test_cases[ii].fill_sp_random();
//        }
//
//        while (!success && test_cases.size() <= 200) {
//            auto result = opt_once(prog, test_cases, program.get<int>("--time-per-opt"),1);
//            if (result.has_value()) {
//                // find a solution
//                auto rewrite = result.value();
//                auto verifier = megumin::SymbolicVerifier(error_file);
//                auto verify_result = verifier.verify(prog, rewrite);
//                if (verify_result.success) {
//                    success = true;
//
//                    correct_file << "[optimization success]" << endl;
//                    correct_file << viable_bbs[i].get_start() << ", " << viable_bbs[i].get_end() << endl;
//                    prog.print(correct_file);
//                    correct_file << endl << ">>>" << endl;
//                    rewrite.print(correct_file);
//                    correct_file << "\n\n";
//                    correct_file.flush();
//
//                    cout << "[optimization success]" << endl;
//                    prog.print(cout);
//                    cout << endl << ">>>" << endl;
//                    rewrite.print(cout);
//                    cout << "\n\n";
//                } else {
//                    if (verify_result.counter_example.has_value()) {
//                        cout << "found counter example:" << endl;
//                        test_cases.push_back(verify_result.counter_example.value());
//                    }
//                }
//            } else {
//                // did not find a solution
//                break;
//            }
//        }
//    }

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