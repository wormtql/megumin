//
// Created by 58413 on 2023/4/18.
//

#include <vector>
#include <limits>
#include <iostream>
#include <bitset>

#include <MachineState.h>
#include "CorrectnessCost.h"

using arm::bits;
using namespace std;

int count_set_bits(int64_t n) {
    // printf("%llx\n", n);
    // cout << bitset<64>(n) << endl;
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

int eval_distance(const arm::bits &v1, const arm::bits &v2) {
    int64_t a = v1.data0;
    int64_t b = v2.data0;

    // hamming distance
    // cout << a << ", " << b << endl;

    return count_set_bits(a ^ b);
}

double ulp_distance(const arm::bits& v1, const arm::bits& v2) {
    int64_t a = v1.data0;
    int64_t b = v2.data0;

    a = a < 0 ? std::numeric_limits<int64_t>::min() - a : a;
    b = b < 0 ? std::numeric_limits<int64_t>::min() - b : b;

    double ulp = a > b ? (double) a - b : (double) b - a;
    // cout << "ulp: " << ulp << endl;
//    if (ulp > 0) {
//        assert(false);
//    }
    return ulp;
}

namespace megumin {
    CorrectnessCost::CorrectnessCost(const arm::Program &target, std::vector<arm::MachineState>&& test_cases)
        : test_cases(test_cases) {
        for (int i = 0; i < test_cases.size(); i++) {
            arm::MachineState state{test_cases[i]};
            target.execute(state);
            target_states.push_back(state);
        }
    }

    CorrectnessCost::CorrectnessCost(vector<arm::MachineState>&& test_cases, vector<arm::MachineState> &&target_states)
        : target_states(target_states), test_cases(test_cases)
    {}

    double CorrectnessCost::calc_single_cost(const arm::MachineState &target_state,
                                                      const arm::MachineState &rewrite_state) const {
        double result = 0;

        // todo extract few registers instead of all registers
        for (int i = 0; i < 32; i++) {
            const bits& reg_target = target_state.gp.get_ref(i);
            const bits& reg_rewrite = rewrite_state.gp.get_ref(i);
            result += eval_distance(reg_target, reg_rewrite);
        }

        // calc floating point distance
        // todo high 64 bits
        for (int i = 0; i < 32; i++) {
            const bits& reg_target = target_state.fp.get_ref(i, true);
            const bits& reg_rewrite = rewrite_state.fp.get_ref(i, true);
            result += ulp_distance(reg_target, reg_rewrite);
        }

        // nzcv cost
        if (target_state.p_state.n != rewrite_state.p_state.n) {
            result++;
        }
        if (target_state.p_state.z != rewrite_state.p_state.z) {
            result++;
        }
        if (target_state.p_state.c != rewrite_state.p_state.c) {
            result++;
        }
        if (target_state.p_state.v != rewrite_state.p_state.v) {
            result++;
        }

        // sp cost
        result += eval_distance(target_state.sp, rewrite_state.sp);

        // cout << "result: " << result << endl;
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
