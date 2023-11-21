//
// Created by 58413 on 2023/5/11.
//

#ifndef MEGUMIN_RANDOMFPANDSIMD_H
#define MEGUMIN_RANDOMFPANDSIMD_H

#include <memory>
#include <random>
#include <random_instruction/RandomInstruction.h>

namespace megumin {
    // random fp 1-source
    // https://developer.arm.com/documentation/ddi0596/2021-12/Index-by-Encoding/Data-Processing----Scalar-Floating-Point-and-Advanced-SIMD?lang=en#floatdp1
    class RandomFPDataProcessing1: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomFPDataProcessing1(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
    };

    // random fp 2-source
    // https://developer.arm.com/documentation/ddi0596/2021-12/Index-by-Encoding/Data-Processing----Scalar-Floating-Point-and-Advanced-SIMD?lang=en#floatdp2
    class RandomFPDataProcessing2: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomFPDataProcessing2(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
    };

    class RandomFPDataProcessing3: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomFPDataProcessing3(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program& program, int index) override;
    };

    class RandomFPImm: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomFPImm(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction(const arm::Program &program, int index) override;
    };
}


#endif //MEGUMIN_RANDOMFPANDSIMD_H
