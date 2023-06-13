//
// Created by 58413 on 2023/4/19.
//

#include "SimpleCost.h"

namespace megumin {
    SimpleCost::SimpleCost(const arm::Program &target, vector<arm::MachineState> &&test_cases)
        : length_cost(), correctness_cost(target, std::move(test_cases))
    {}

    SimpleCost::SimpleCost(vector<arm::MachineState>&& start_states, vector<arm::MachineState>&& end_states)
        : length_cost(), correctness_cost{std::move(start_states), std::move(end_states)}
    {}

    std::pair<CostFunction::CorrectState, double> megumin::SimpleCost::cost(const arm::Program &program, double max_cost) const {
        const auto result1 = correctness_cost.cost(program, max_cost);
        const auto result2 = length_cost.cost(program, max_cost);

        return {
            result1.first,
            result1.second + result2.second
        };
    }
}
