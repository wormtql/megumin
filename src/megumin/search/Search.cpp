//
// Created by 58413 on 2023/4/17.
//

#include <iostream>
#include <cmath>
#include <chrono>
#include "Search.h"
#include "Program.h"

using namespace std;

namespace megumin {
    Search::Search(ProgramMutation *program_mutation, CostFunction *cost_function, std::mt19937 &generator)
        : generator(generator), program_mutation(program_mutation), cost_function(cost_function)
    {}

    void Search::do_search(SearchState &state) {
        auto start = std::chrono::steady_clock::now();
        for (unsigned long long i = 0; i < max_iteration; i++) {
            auto now = std::chrono::steady_clock::now();
            auto duration = now - start;
            auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

            if (duration_ms > max_time) {
                break;
            }

            arm::Program& program = state.current;
            auto mutation_result = program_mutation->mutate(program);

            const double p = uniform_distribution(generator);
            const double max_cost = state.current_cost - (std::log(p) / beta);
            const auto new_cost_result = cost_function->cost(program, max_cost + 1);

            const bool is_correct = new_cost_result.first == CostFunction::CorrectState::Correct;
            const double new_cost = new_cost_result.second;


            if (new_cost > max_cost) {
                program_mutation->undo(program, mutation_result);
                continue;
            }
            state.current_cost = new_cost;
//            state.current = new_program;

            if (state.current_best_cost > new_cost) {

                cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << endl;
                state.current_best_cost = new_cost;
                state.current_best = state.current;
                std::cout << "iteration: " << i << "\n";
                state.current.print();
                std::cout << "\n";
                std::cout << new_cost << "\n\n";
                std::flush(std::cout);
            }

            if (new_cost < state.current_correct_best_cost && is_correct) {
                state.current_correct_best = state.current;
                state.current_correct_best_cost = new_cost;
                state.success = true;

                cout << "[correct]\n";
                cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << endl;
                std::cout << "iteration: " << i << "\n";
                state.current_correct_best.print();
                std::cout << "\n";
                std::cout << new_cost << "\n\n";
                std::flush(std::cout);
            }
        }
    }
}