//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_MUTATIONRESULT_H
#define MEGUMIN_MUTATIONRESULT_H

#include "Instruction.h"
#include "Program.h"

namespace megumin {
    struct MutationResult {
        bool success = false;

        arm::Program::ProgramPosition mutation_index[2];
        arm::Instruction mutation_instructions[2];
        int tag = 0;
    };
}



#endif //MEGUMIN_MUTATIONRESULT_H
