//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_PROGRAMMUTATION_H
#define MEGUMIN_PROGRAMMUTATION_H

#include "MutationResult.h"
#include "Program.h"
#include "megumin_utils.h"

namespace megumin {
    class ProgramMutation {
    public:
        virtual ~ProgramMutation() = default;

        virtual MutationResult mutate(arm::Program& program) = 0;

        virtual void undo(arm::Program& program, const MutationResult& result) = 0;
    };
}



#endif //MEGUMIN_PROGRAMMUTATION_H
