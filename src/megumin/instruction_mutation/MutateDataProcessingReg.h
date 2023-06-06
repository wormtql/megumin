//
// Created by 58413 on 2023/4/22.
//

#ifndef MEGUMIN_MUTATEDATAPROCESSINGREG_H
#define MEGUMIN_MUTATEDATAPROCESSINGREG_H

#include <random>
#include "instruction_mutation/InstructionMutation.h"
#include "Program.h"

namespace megumin {
    class MutateDataProcessingReg2Source: public InstructionMutation {
    public:
        struct Prob {
            double w_opcode = 2.5;
            double w_sf = 1;
            double w_rm = 5;
            double w_rn = 5;
            double w_rd = 5;
        };
    private:
        arm::Instruction mutate_opcode(const arm::Program& program, int index);
        arm::Instruction mutate_sf(const arm::Program& program, int index);
        arm::Instruction mutate_rm(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
        std::discrete_distribution<> discrete;
    public:
        MutateDataProcessingReg2Source(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingReg2Source(std::mt19937& generator): MutateDataProcessingReg2Source(generator, {}) {}
        arm::Instruction mutate(const arm::Program& program, int index) override;

    };

    class MutateDataProcessingReg1Source: public InstructionMutation {
    public:
        struct Prob {
            double w_sf = 1;
            double w_opcode = 2;
            double w_rn = 5;
            double w_rd = 5;
        };
    private:
        arm::Instruction mutate_sf(const arm::Program& program, int index);
        arm::Instruction mutate_opcode(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
        std::discrete_distribution<> discrete;
    public:
        MutateDataProcessingReg1Source(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingReg1Source(std::mt19937& generator): MutateDataProcessingReg1Source(generator, {}) {}

        arm::Instruction mutate(const arm::Program& program, int index) override;
    };

    class MutateDataProcessingRegLogical: public InstructionMutation {
    public:
        struct Prob {
            double w_sf = 0.1;
            double w_opc = 1;
            double w_shift = 1;
            double w_N = 1;
            double w_rm = 5;
            double w_imm6 = 1;
            double w_rn = 5;
            double w_rd = 5;
        };
    private:
        arm::Instruction mutate_opc(const arm::Program& program, int index);
        arm::Instruction mutate_shift(const arm::Program& program, int index);
        arm::Instruction mutate_N(const arm::Program& program, int index);
        arm::Instruction mutate_imm6(const arm::Program& program, int index);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGREG_H
