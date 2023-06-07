//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_RANDOMINSTRUCTION_H
#define MEGUMIN_RANDOMINSTRUCTION_H

#include <random>
#include <Instruction.h>
#include "Program.h"

namespace megumin {
    class RandomInstruction {
    protected:
        static std::uniform_int_distribution<> uniform_int;
        static std::mt19937 generator;

        static std::uniform_int_distribution<>::result_type r() {
            return uniform_int(generator);
        }
    public:
        virtual ~RandomInstruction() = default;

        virtual arm::Instruction random_instruction(const arm::Program& program, int index) = 0;
    };
}



#endif //MEGUMIN_RANDOMINSTRUCTION_H
