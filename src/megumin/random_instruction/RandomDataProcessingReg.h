//
// Created by 58413 on 2023/4/22.
//

#ifndef MEGUMIN_RANDOMDATAPROCESSINGREG_H
#define MEGUMIN_RANDOMDATAPROCESSINGREG_H

#include <random>
#include <memory>
#include <vector>
#include "random_instruction/RandomInstruction.h"

namespace megumin {
    class RandomDataProcessing2Source: public RandomInstruction {
    private:
        std::uniform_int_distribution<> uniform_int;
        std::mt19937& generator;
    public:
        explicit RandomDataProcessing2Source(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingReg: public RandomInstruction {
    public:
        struct Prob {
            double w_2_source = 1.0;
        };
    private:
        std::discrete_distribution<> discrete;
        std::mt19937& generator;
        std::vector<std::unique_ptr<RandomInstruction>> dispatches;
    public:
        RandomDataProcessingReg(std::mt19937& generator, Prob prob);
        explicit RandomDataProcessingReg(std::mt19937& generator): RandomDataProcessingReg(generator, {}) {}
        arm::Instruction random_instruction() override;
    };
}

#endif //MEGUMIN_RANDOMDATAPROCESSINGREG_H
