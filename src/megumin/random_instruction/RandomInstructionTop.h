//
// Created by 58413 on 2023/4/22.
//

#ifndef MEGUMIN_RANDOMINSTRUCTIONTOP_H
#define MEGUMIN_RANDOMINSTRUCTIONTOP_H

#include <random>
#include <vector>
#include <memory>
#include "random_instruction/RandomInstruction.h"

namespace megumin {
    class RandomInstructionTop: public RandomInstruction {
    public:
        struct Prob {
            double w_data_processing_imm = 1.0;
            double w_data_processing_reg = 1.0;
            double w_fp_and_simd = 1.0;
        };
    private:
        std::mt19937& generator;
        std::discrete_distribution<> discrete;
        std::vector<std::unique_ptr<RandomInstruction>> dispatches;
    public:
        RandomInstructionTop(std::mt19937& generator, Prob prob);
        explicit RandomInstructionTop(std::mt19937& generator): RandomInstructionTop(generator, {}) {}

        arm::Instruction random_instruction() override;
    };
}



#endif //MEGUMIN_RANDOMINSTRUCTIONTOP_H
