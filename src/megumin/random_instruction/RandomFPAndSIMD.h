//
// Created by 58413 on 2023/5/11.
//

#ifndef MEGUMIN_RANDOMFPANDSIMD_H
#define MEGUMIN_RANDOMFPANDSIMD_H

#include <random>
#include <random_instruction/RandomInstruction.h>

namespace megumin {
    class RandomFPDataProcessing1: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomFPDataProcessing1(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction() override;
    };

    // top
    class RandomFPAndSIMD: public RandomInstruction {
    public:
        struct Prob {
            double w_fp_data_processing1 = 1.0;
        };
    private:
        std::mt19937& generator;
        std::discrete_distribution<> discrete;

        std::vector<std::unique_ptr<RandomInstruction>> dispatches;
    public:
        RandomFPAndSIMD(std::mt19937& generator, Prob prob);
        explicit RandomFPAndSIMD(std::mt19937& generator): RandomFPAndSIMD(generator, {}) {}

        arm::Instruction random_instruction() override;
    };
}


#endif //MEGUMIN_RANDOMFPANDSIMD_H
