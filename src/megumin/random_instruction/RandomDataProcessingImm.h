//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_RANDOMDATAPROCESSINGIMM_H
#define MEGUMIN_RANDOMDATAPROCESSINGIMM_H

#include <random>
#include <memory>
#include <vector>
#include "random_instruction/RandomInstruction.h"

namespace megumin {
    class RandomAddSubImm: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int_dist;
    public:
        explicit RandomAddSubImm(std::mt19937& generator);
        arm::Instruction random_instruction(const arm::Program& program, arm::Program::ProgramPosition position) override;
    };

    class RandomDataProcessingImmLogical: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int_distribution;
    public:
        explicit RandomDataProcessingImmLogical(std::mt19937& generator): generator(generator) {}
        arm::Instruction random_instruction(const arm::Program& program, arm::Program::ProgramPosition position) override;
    };

    class RandomMoveWideImm: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomMoveWideImm(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, arm::Program::ProgramPosition position) override;
    };

    class RandomBitfield: public RandomInstruction {
    private:
        std::mt19937 &generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomBitfield(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, arm::Program::ProgramPosition position) override;
    };

    class RandomExtract: public RandomInstruction {
    private:
        std::mt19937 &generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomExtract(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, arm::Program::ProgramPosition position) override;
    };
}



#endif //MEGUMIN_RANDOMDATAPROCESSINGIMM_H
