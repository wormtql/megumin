//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_CORRECTNESSCOST_H
#define MEGUMIN_CORRECTNESSCOST_H

#include <vector>

#include <Bitvec.h>
#include "cost/CostFunction.h"

using std::vector;

namespace megumin {
    class CorrectnessCost: public CostFunction {
    private: arm::Program target;
    private: vector<arm::MachineState> target_states;
    private: vector<arm::MachineState> test_cases;

    public: explicit CorrectnessCost(const arm::Program& target, vector<arm::MachineState>&& test_cases);

    public: [[nodiscard]] std::pair<CorrectState, double> cost(const arm::Program &program, double max_cost) const override;

    private: [[nodiscard]] double calc_single_cost(const arm::MachineState& target_state, const arm::MachineState& rewrite_state) const;
    };
}


#endif //MEGUMIN_CORRECTNESSCOST_H
