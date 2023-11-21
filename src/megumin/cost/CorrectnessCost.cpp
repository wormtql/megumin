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

namespace megumin {
    uint64_t ulp_distance(const arm::bits& v1, const arm::bits& v2) {
        int64_t a = v1.data0;
        int64_t b = v2.data0;

        a = a < 0 ? std::numeric_limits<int64_t>::min() - a : a;
        b = b < 0 ? std::numeric_limits<int64_t>::min() - b : b;

        uint64_t ulp = a >= b ? a - b : b - a;
//    ulp = ulp < min_ulp_ ? 0 : ulp - min_ulp_;
//    double ulp = a > b ? (double) a - b : (double) b - a;
        // cout << "ulp: " << ulp << endl;
//    if (ulp > 0) {
//        assert(false);
//    }
        return ulp;
    }

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
            auto ulp = ulp_distance(reg_target, reg_rewrite);
            if (ulp < min_ulp_error) {
                ulp = 0;
            }
            result += ulp;
        }

        // nzcv cost
        if (target_state.p_state.n != rewrite_state.p_state.n) {
            result += 1;
        }
        if (target_state.p_state.z != rewrite_state.p_state.z) {
            result += 2;
        }
        if (target_state.p_state.c != rewrite_state.p_state.c) {
            result += 3;
        }
        if (target_state.p_state.v != rewrite_state.p_state.v) {
            result += 4;
        }

        // sp cost
        result += eval_distance(target_state.sp, rewrite_state.sp);

        // cout << "result: " << result << endl;
        return result;
    }

    std::pair<CostFunction::CorrectState, double> CorrectnessCost::cost(const arm::Program &program, double max_cost) const {
        double cost = 0.0;
//        int it = 0;
//        while ((1 << it) <= test_cases.size()) {
//            double temp_cost = 0.0;
//            for (int i = 0; i < (1 << it); i++) {
//                arm::MachineState state{test_cases[i]};
//                program.execute(state);
//
//                temp_cost += calc_single_cost(target_states[i], state);
//            }
//            temp_cost /= (1 << it);
//            cost = temp_cost;
//            if (temp_cost >= max_cost) {
//                return { CorrectState::Wrong, max_cost };
//            }
//            if (temp_cost > 0.0) {
//                return { CorrectState::Wrong, temp_cost };
//            } else {
////                cout << "correct " << (1 << it) << endl;
//                it++;
//            }
//        }
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
//            cout << "correct" << cost << endl;
            return { CorrectState::Correct, cost };
        }
    }
}
