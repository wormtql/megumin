//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_RANDOMINSTRUCTIONMUTATION_H
#define MEGUMIN_RANDOMINSTRUCTIONMUTATION_H

#include <random>
#include <memory>
#include <vector>
#include "ProgramMutation.h"
#include "random_instruction/RandomInstruction.h"

namespace megumin {
    class RandomInstructionMutation: public ProgramMutation {
    public:
        struct RandomInstructionWeight {
            int dp_imm_add_sub = 1;
            int dp_imm_add_sub_with_tags = 0;
            int dp_imm_logical = 1;
            int dp_imm_move_wide = 1;
            int dp_imm_bitfield = 1;
            int dp_imm_extract = 1;

            int dp_reg_1source = 1;
            int dp_reg_2source = 1;
            int dp_reg_logical_shifted_reg = 1;
            int dp_reg_add_sub_shifted_reg = 1;

            int fp_and_simd_dp_1source = 1;
            int fp_and_simd_dp_2source = 1;
        };
    private:
        std::uniform_int<> uniform_int;
        std::mt19937& generator;

        std::vector<std::unique_ptr<RandomInstruction>> random_functions;
    public:
        explicit RandomInstructionMutation(std::mt19937& generator, RandomInstructionWeight weight);

        MutationResult mutate(arm::Program &program) override;

        void undo(arm::Program &program, const MutationResult &result) override;
    };
}



#endif //MEGUMIN_RANDOMINSTRUCTIONMUTATION_H
