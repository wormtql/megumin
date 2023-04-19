//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_LENGTHCOST_H
#define MEGUMIN_LENGTHCOST_H

#include <Program.h>
#include "cost/CostFunction.h"

namespace megumin {
    class LengthCost: CostFunction {
    public:
        [[nodiscard]] std::pair<CorrectState, double> cost(const arm::Program &program, double max_cost) const override;
    };
}

#endif //MEGUMIN_LENGTHCOST_H
