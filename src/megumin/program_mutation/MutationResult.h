//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_MUTATIONRESULT_H
#define MEGUMIN_MUTATIONRESULT_H

#include "Instruction.h"

namespace megumin {
    struct MutationResult {
        bool success = false;

        int mutation_index[2];
        arm::Instruction mutation_instructions[2];
    };
}



#endif //MEGUMIN_MUTATIONRESULT_H
