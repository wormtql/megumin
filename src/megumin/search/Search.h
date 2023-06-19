//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_SEARCH_H
#define MEGUMIN_SEARCH_H

#include <random>

#include "program_mutation/ProgramMutation.h"
#include "cost/CostFunction.h"

namespace megumin {
    class SearchState {
    public:
        arm::Program current_best;
        double current_best_cost;

        arm::Program current_correct_best;
        double current_correct_best_cost;

        arm::Program current;
        double current_cost;

        bool success = false;
    };

    class Search {
    private:
        ProgramMutation* program_mutation;
        CostFunction* cost_function;
        double beta = 1.0;
        unsigned long long max_iteration = 10000000000;
        // in ms
        int max_time = 60000;

        std::mt19937& generator;
        std::uniform_real_distribution<double> uniform_distribution;
    public:
        Search(ProgramMutation* program_mutation, CostFunction* cost_function, std::mt19937& generator);

        inline void set_beta(double b) {
            this->beta = b;
        }

        inline void set_max_iteration(int it) {
            this->max_iteration = it;
        }

        inline void set_max_time(int t) {
            this->max_time = t;
        }

        void do_search(SearchState& state);
    };
}



#endif //MEGUMIN_SEARCH_H
