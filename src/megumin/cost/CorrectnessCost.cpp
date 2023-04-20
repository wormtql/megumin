//
// Created by 58413 on 2023/4/18.
//

#include <vector>

#include <MachineState.h>
#include "CorrectnessCost.h"

using arm::bits;

int count_set_bits(int64_t n) {
    auto t = static_cast<uint64_t>(n);
    int result = 0;
    while (t != 0) {
        if (t & 1) {
            result++;
        }
        t >>= 1;
    }
    return result;
}

double eval_distance(const arm::bits &v1, const arm::bits &v2) {
    int64_t a = v1.data0;
    int64_t b = v2.data0;

    // hamming distance
    return count_set_bits(a ^ b);
}

namespace megumin {
    CorrectnessCost::CorrectnessCost(const arm::Program &target, std::vector<arm::MachineState>&& test_cases)
        : target(target), test_cases(test_cases) {
        for (int i = 0; i < test_cases.size(); i++) {
            arm::MachineState state{test_cases[i]};
            target.execute(state);
            target_states.push_back(state);
        }
    }

    double CorrectnessCost::calc_single_cost(const arm::MachineState &target_state,
                                                      const arm::MachineState &rewrite_state) const {
        double result = 0;

        // todo extract few registers instead of all registers
        for (int i = 0; i < 32; i++) {
            const bits& reg_target = target_state.gp.get_ref(i);
            const bits& reg_rewrite = rewrite_state.gp.get_ref(i);
            result += eval_distance(reg_target, reg_rewrite);
        }
        return result;
    }

    std::pair<CostFunction::CorrectState, double> CorrectnessCost::cost(const arm::Program &program, double max_cost) const {
        double cost = 0.0;
        for (int i = 0; i < test_cases.size(); i++) {
            arm::MachineState state{test_cases[i]};
            program.execute(state);

            cost += calc_single_cost(target_states[i], state);
            if (cost >= max_cost) {
                return { CorrectState::Wrong, max_cost };
            }
        }

        if (cost > 0) {
            return { CorrectState::Wrong, cost };
        } else {
            return { CorrectState::Unknown, cost };
        }
    }
}
