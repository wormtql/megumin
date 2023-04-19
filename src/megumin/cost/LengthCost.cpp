//
// Created by 58413 on 2023/4/18.
//

#include "LengthCost.h"

namespace megumin {
    std::pair<CostFunction::CorrectState, double> LengthCost::cost(const arm::Program &program, double max_cost) const {
        int length = 0;

        for (int i = 0, len = program.get_size(); i < len; i++) {
            if (!program.get_instruction_const(i).is_nop()) {
                length++;
            }
        }

        if (length >= max_cost) {
            return { CorrectState::Unknown, max_cost };
        }
        return { CorrectState::Unknown, length };
    }
}
