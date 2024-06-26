//
// Created by 58413 on 2023/4/22.
//

#ifndef MEGUMIN_RANDOMDATAPROCESSINGREG_H
#define MEGUMIN_RANDOMDATAPROCESSINGREG_H

#include <random>
#include <memory>
#include <vector>
#include "random_instruction/RandomInstruction.h"
#include "Program.h"

namespace megumin {
    class RandomDataProcessing2Source: public RandomInstruction {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit RandomDataProcessing2Source(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessing1Source: public RandomInstruction {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit RandomDataProcessing1Source(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingRegLogical: public RandomInstruction {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit RandomDataProcessingRegLogical(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingRegAddSubShiftedReg: public RandomInstruction {
    public:
        arm::Instruction random_instruction(const arm::Program &program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingRegAddSubWithCarry: public RandomInstruction {
    public:
        arm::Instruction random_instruction(const arm::Program &program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingRegCondSelect: public RandomInstruction {
    public:
        arm::Instruction random_instruction(const arm::Program &program, int index) override;
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessing3Source: public RandomInstruction {
    public:
        arm::Instruction random_instruction(const arm::Program &program, int index) override;
        arm::Instruction random_instruction() override;
    };
}

#endif //MEGUMIN_RANDOMDATAPROCESSINGREG_H
