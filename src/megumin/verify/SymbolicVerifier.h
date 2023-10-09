//
// Created by 58413 on 2023/7/3.
//

#ifndef MEGUMIN_SYMBOLICVERIFIER_H
#define MEGUMIN_SYMBOLICVERIFIER_H

#include <string>
#include <optional>
#include "Verifier.h"

using namespace std;

namespace megumin {
    class SymbolicVerifier: Verifier {
    public:
        struct SymbolicVerifyDebugInfo {
            string reason;
            arm::MachineState counter_example;
            arm::MachineState test_state1;
            arm::MachineState test_state2;
            arm::MachineState symbolic_state1;
            arm::MachineState symbolic_state2;
            arm::Program target;
            arm::Program rewrite;
        };

        optional<SymbolicVerifyDebugInfo> error_debug_info = {};

        explicit SymbolicVerifier() = default;

        VerifyResult verify(const arm::Program &target, const arm::Program &rewrite) override;
    };

    ostream& operator<<(ostream& os, const SymbolicVerifier::SymbolicVerifyDebugInfo& debug_info);
}



#endif //MEGUMIN_SYMBOLICVERIFIER_H
