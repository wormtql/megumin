//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_RANDOMINSTRUCTION_H
#define MEGUMIN_RANDOMINSTRUCTION_H

#include <Instruction.h>

namespace megumin {
    class RandomInstruction {
    public:
        virtual ~RandomInstruction() = default;

        virtual arm::Instruction random_instruction() = 0;
    };
}



#endif //MEGUMIN_RANDOMINSTRUCTION_H
