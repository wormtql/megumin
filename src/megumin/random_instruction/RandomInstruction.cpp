//
// Created by 58413 on 2023/6/7.
//

#include "RandomInstruction.h"

namespace megumin {
    std::uniform_int_distribution<> RandomInstruction::uniform_int;

    std::mt19937 RandomInstruction::generator{100};
}
