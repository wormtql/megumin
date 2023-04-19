//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_COSTFUNCTION_H
#define MEGUMIN_COSTFUNCTION_H

#include <vector>
#include <Program.h>

namespace megumin {
    class CostFunction {
    public:
        enum class CorrectState {
            Unknown,
            Correct,
            Wrong,
        };

        virtual ~CostFunction() = default;

        [[nodiscard]] virtual std::pair<CorrectState, double> cost(const arm::Program& program, double max_cost = -1) const = 0;
    };
}



#endif //MEGUMIN_COSTFUNCTION_H
