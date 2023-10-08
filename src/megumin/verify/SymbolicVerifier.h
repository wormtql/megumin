//
// Created by 58413 on 2023/7/3.
//

#ifndef MEGUMIN_SYMBOLICVERIFIER_H
#define MEGUMIN_SYMBOLICVERIFIER_H

#include <optional>
#include "Verifier.h"

namespace megumin {
    class SymbolicVerifier: Verifier {
    private:
        std::ostream& error_output_stream;
    public:
        explicit SymbolicVerifier(std::ostream& os);

        VerifyResult verify(const arm::Program &target, const arm::Program &rewrite) override;
    };
}



#endif //MEGUMIN_SYMBOLICVERIFIER_H
