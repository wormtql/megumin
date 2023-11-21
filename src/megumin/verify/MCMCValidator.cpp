//
// Created by 58413 on 2023/10/18.
//

#include <random>
#include <numeric>
#include "MCMCValidator.h"
#include "cost/CorrectnessCost.h"

using namespace std;

namespace megumin {
    int ulp_cost(const arm::FPRegBank& bank1, const arm::FPRegBank& bank2) {
//        int cost = 0;
//        for (int i = 0; i < 32; i++) {
//            for (int j = 0; j < 2; j++) {
//
//            }
//            cost += ulp_distance(bank1.get)
//        }
        return 0;
    }

    void MCMCValidator::MCMCValidatorInput::fill_random() {
        uniform_int_distribution<int> uniform_int{numeric_limits<int>::min(), numeric_limits<int>::max()};
        mt19937 generator{1000};
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 2; j++) {
                fp.get_mut_ref(i, j).set_range(0, 32, uniform_int(generator));
                fp.get_mut_ref(i, j).set_range(32, 64, uniform_int(generator));
            }
        }
    }

    VerifyResult MCMCValidator::verify(const arm::Program &target, const arm::Program &rewrite) {
        MCMCValidatorInput current;
        current.fill_random();


        for (unsigned long long i = 0; i < max_iterations; i++) {


        }

        VerifyResult result{false, {}};
        return result;
    }
}