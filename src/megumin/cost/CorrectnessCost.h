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
    uint64_t ulp_distance(const arm::bits& v1, const arm::bits& v2);

    class CorrectnessCost: public CostFunction {
    private:
        vector<arm::MachineState> target_states;
        vector<arm::MachineState> test_cases;

    public:
        explicit CorrectnessCost(const arm::Program& target, vector<arm::MachineState>&& test_cases);
        explicit CorrectnessCost(vector<arm::MachineState>&& test_cases, vector<arm::MachineState>&& target_states);

        [[nodiscard]] std::pair<CorrectState, double> cost(const arm::Program &program, double max_cost) const override;

        [[nodiscard]] double calc_single_cost(const arm::MachineState& target_state, const arm::MachineState& rewrite_state) const;
    };
}


#endif //MEGUMIN_CORRECTNESSCOST_H
