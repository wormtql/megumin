//
// Created by 58413 on 2023/4/18.
//

#include "LengthCost.h"

namespace megumin {
    std::pair<CostFunction::CorrectState, double> LengthCost::cost(const arm::Program &program, double max_cost) const {
        int length = program.get_non_nop_size();

        if (length >= max_cost) {
            return { CorrectState::Unknown, max_cost };
        }
        return { CorrectState::Unknown, length };
    }
}
