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
#include <memory>

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

#include <BS_thread_pool.hpp>

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
    bool all_integral = target.is_all_integral_instructions();
    if (all_integral) {
        search.set_use_fp(false);
    } else {
        search.set_use_fp(true);
    }
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
mutex g_progress_data;

struct GlobalData {
    shared_ptr<ostream> error_stream;
    shared_ptr<ostream> correct_stream;
    shared_ptr<ostream> progress_stream;
    int finished_bb = 0;
    int total_bb = 0;
    vector<int> restart_times;
};

optional<SearchResult> worker(const BasicBlock* bb_ptr, int time_per_opt, GlobalData* global_data) {
    bool success = false;
    const BasicBlock& basic_block = *bb_ptr;
    Program prog = basic_block.to_program();

    {
        lock_guard<mutex> progress_guard(g_progress_data);
        cout << basic_block;
    }

    optional<SearchResult> ret_value;

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

    int max_test_case_count = 2000;
    int iter_count = 0;
    while (!success && test_cases.size() <= max_test_case_count) {
        iter_count++;
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
                (*global_data->error_stream) << verifier.error_debug_info.value() << endl;
            }

            if (verify_result.success) {
                success = true;

                SearchResult search_result{ prog, rewrite, basic_block.get_start(), basic_block.get_end() };
                ret_value = search_result;

                // write correct stream
                {
                    lock_guard<mutex> guard(g_correct_stream);

                    ostream& cs = *global_data->correct_stream;

                    cs << "[optimization success]" << endl;
                    cs << basic_block.get_start() << ", " << basic_block.get_end() << endl;
                    prog.print(cs);
                    cs << endl << ">>>" << endl;
                    rewrite.print(cs);
                    cs << "\n\n";
                    cs.flush();
                }

                // write standard output
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

    lock_guard<mutex> progress_guard(g_progress_data);
    global_data->finished_bb++;
    printf("progress: %d/%d(%.2lf%%)\n", global_data->finished_bb, global_data->total_bb, (double)global_data->finished_bb / global_data->total_bb * 100);
    global_data->restart_times.push_back(iter_count);
    (*global_data->progress_stream) << basic_block.get_start() << " " << basic_block.get_end() << endl;

    return ret_value;
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("megumin auto opt");
    // save optimization results
    program.add_argument("--correct-file").default_value("correct.txt");
    // input assembly file
    program.add_argument("--input-file");
    // error recording file
    program.add_argument("--error-file").default_value("error.txt");
    // time spent on a single search
    program.add_argument("--time-per-opt").default_value(10000).scan<'i', int>();
    // threads used
    program.add_argument("--thread-count").default_value(1).scan<'i', int>();
    program.add_argument("--seed").default_value(10086).scan<'i', int>();

    program.parse_args(argc, argv);

    shared_ptr<ofstream> correct_file = make_shared<ofstream>(program.get<std::string>("--correct-file"));
    BBExtractor extractor{program.get<string>("--input-file")};
    shared_ptr<ofstream> error_file = make_shared<ofstream>(program.get<string>("--error-file"));
    shared_ptr<ofstream> progress_file = make_shared<ofstream>("progress.txt");

    if (!correct_file->is_open()) {
        cout << "cannot open " << program.get<string>("--correct-file") << endl;
        return 0;
    }
    if (!error_file->is_open()) {
        cout << "cannot open " << program.get<string>("--error-file") << endl;
        return 0;
    }
    if (!progress_file->is_open()) {
        cout << "cannot open" << "progress.txt" << endl;
        return 0;
    }

    extractor.set_max_bb(-1);
    vector<BasicBlock> bbs = extractor.extract_basic_blocks();

    vector<BasicBlock> viable_bbs;
    for (const auto& bb: bbs) {
        if (can_process(bb)) {
            viable_bbs.push_back(bb);
        }
    }

    int viable_bb_size = static_cast<int>(viable_bbs.size());
    cout << "viable basic block count: " << viable_bb_size << endl;

    int thread_count = program.get<int>("--thread-count");
    int time_per_opt = program.get<int>("--time-per-opt");
    int seed = program.get<int>("--seed");
    GlobalData global_data = {
        .error_stream = dynamic_pointer_cast<ostream>(error_file),
        .correct_stream = dynamic_pointer_cast<ostream>(correct_file),
        .progress_stream = dynamic_pointer_cast<ostream>(progress_file),
        .finished_bb = 0,
        .total_bb = viable_bb_size
    };

    BS::thread_pool pool{static_cast<unsigned int>(thread_count)};
    const auto fut = pool.submit_loop(0, viable_bb_size, [&] (int i) {
        worker(&viable_bbs[i], time_per_opt, &global_data);
    }, viable_bb_size);

    fut.wait();

    // calculate average restart times
    int sum = 0;
    for (int t: global_data.restart_times) {
        sum += t;
    }
    double avg = (double)sum / (double)global_data.restart_times.size();
    cout << "average restart time: " << avg << endl;

    return 0;
}