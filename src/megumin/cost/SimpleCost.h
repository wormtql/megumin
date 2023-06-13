//
// Created by 58413 on 2023/4/19.
//

#ifndef MEGUMIN_SIMPLECOST_H
#define MEGUMIN_SIMPLECOST_H

#include <vector>
#include <Program.h>
#include "cost/CostFunction.h"
#include "cost/LengthCost.h"
#include "cost/CorrectnessCost.h"

namespace megumin {
    class SimpleCost: public CostFunction {
    private:
        LengthCost length_cost;
        CorrectnessCost correctness_cost;
    public:
        SimpleCost(const arm::Program& target, vector<arm::MachineState>&& test_cases);
        SimpleCost(vector<arm::MachineState>&& start_states, vector<arm::MachineState>&& end_states);

        [[nodiscard]] std::pair<CorrectState, double> cost(const arm::Program &program, double max_cost) const override;
    };
}



#endif //MEGUMIN_SIMPLECOST_H
