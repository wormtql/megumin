//
// Created by 58413 on 2023/6/17.
//

#include "BruteForceVerifier.h"
#include "Verifier.h"

namespace megumin {
    VerifyResult BruteForceVerifier::verify(const arm::Program &target, const arm::Program &rewrite) {
        for (int i = 0; i < test_count; i++) {
            arm::MachineState testcase;
            testcase.fill_random();

            arm::MachineState state(testcase);
            arm::MachineState state2{testcase};

            target.execute(state);
            rewrite.execute(state2);

            if (state != state2) {
                return {false, testcase};
            }
        }

        return {true, {}};
    }
}
