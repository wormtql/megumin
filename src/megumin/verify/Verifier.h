//
// Created by 58413 on 2023/6/17.
//

#ifndef MEGUMIN_VERIFIER_H
#define MEGUMIN_VERIFIER_H

#include <iostream>
#include <optional>
#include <utility>
#include "Program.h"
#include "MachineState.h"

namespace megumin {
    struct VerifyResult {
        bool success = false;
        std::optional<arm::MachineState> counter_example;

        VerifyResult(bool result, std::optional<arm::MachineState> counter)
            : success(result), counter_example(std::move(counter)) {}
    };

    class Verifier {
    public:
        virtual VerifyResult verify(const arm::Program& target, const arm::Program& rewrite) = 0;
    };
}

std::ostream& operator<<(std::ostream& os, const megumin::VerifyResult& result);

#endif //MEGUMIN_VERIFIER_H
