//
// Created by 58413 on 2023/4/17.
//

#include <iostream>
#include <cmath>
#include "Search.h"

using namespace std;

namespace megumin {
    Search::Search(ProgramMutation *program_mutation, CostFunction *cost_function, std::mt19937 &generator)
        : generator(generator), program_mutation(program_mutation), cost_function(cost_function)
    {}

    void Search::do_search(SearchState &state) {
        for (unsigned long long i = 0; i < max_iteration; i++) {
            arm::Program new_program = program_mutation->mutate(state.current);

            const double p = uniform_distribution(generator);
            // cout << "current cost: " << state.current_best_cost << endl;
            const double max_cost = state.current_cost - (std::log(p) / beta);
            // cout << "max_cost: " << max_cost << endl;
            // break;
            const auto new_cost_result = cost_function->cost(new_program, max_cost + 1);

            const bool is_correct = new_cost_result.first == CostFunction::CorrectState::Correct;
            const double new_cost = new_cost_result.second;

            if (i % 100000 == 0) {
                printf("%d: new cost: %lf, current best: %lf\n", i, new_cost, state.current_best_cost);
                new_program.print();
                cout << endl;
                fflush(stdout);
            }

            if (new_cost > max_cost) {
                continue;
            }
            state.current_cost = new_cost;
            state.current = new_program;

            if (state.current_best_cost > new_cost) {
                state.current_best_cost = new_cost;
                state.current_best = new_program;
                std::cout << "iteration: " << i << "\n";
                new_program.print();
                std::cout << "\n";
                std::cout << new_cost << "\n\n";
            }

            if (new_cost < state.current_correct_best_cost && is_correct) {
                state.current_correct_best = new_program;
                state.current_correct_best_cost = new_cost;
            }
        }
    }
}