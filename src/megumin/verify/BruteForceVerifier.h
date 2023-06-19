//
// Created by 58413 on 2023/6/17.
//

#ifndef MEGUMIN_BRUTEFORCEVERIFIER_H
#define MEGUMIN_BRUTEFORCEVERIFIER_H

#include "Verifier.h"

namespace megumin {
    class BruteForceVerifier: public Verifier {
    private:
        int test_count = 1000;
    public:
        explicit BruteForceVerifier(int testcases): test_count(testcases) {}

        VerifyResult verify(const arm::Program &target, const arm::Program &rewrite) override;
    };
}



#endif //MEGUMIN_BRUTEFORCEVERIFIER_H
