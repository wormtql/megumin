//
// Created by 58413 on 2023/10/18.
//

#ifndef MEGUMIN_MCMCVALIDATOR_H
#define MEGUMIN_MCMCVALIDATOR_H

#include <string>
#include <optional>
#include "Verifier.h"
#include <Bitvec.h>

namespace megumin {
    class MCMCValidator: public Verifier {
    private:
        struct MCMCValidatorInput {
            arm::FPRegBank fp;

            void fill_random();
        };

//        struct MCMCValidatorState {
//
//        };

        unsigned long long max_iterations = 100000000;
    public:
        VerifyResult verify(const arm::Program &target, const arm::Program &rewrite) override;
    };
}



#endif //MEGUMIN_MCMCVALIDATOR_H
