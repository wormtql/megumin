//
// Created by 58413 on 2023/6/17.
//

#include "Verifier.h"

std::ostream& operator<<(std::ostream& os, const megumin::VerifyResult& result) {
    os << "VerifyResult {" << result.success;
//    if (result.counter_example.has_value()) {
//        os << ", " << result.counter_example.value();
//    }
    os << "}";
    return os;
}
