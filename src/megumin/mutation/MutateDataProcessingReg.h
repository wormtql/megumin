//
// Created by 58413 on 2023/4/22.
//

#ifndef MEGUMIN_MUTATEDATAPROCESSINGREG_H
#define MEGUMIN_MUTATEDATAPROCESSINGREG_H

#include <random>
#include "mutation/Mutation.h"

namespace megumin {
    class MutateDataProcessingReg2Source: public Mutation {
    public:
        struct Prob {
            double w_opcode = 2.5;
            double w_sf = 1;
            double w_rm = 5;
            double w_rn = 5;
            double w_rd = 5;
        };
    private:
        arm::Instruction mutate_opcode(const arm::Instruction& instruction);
        arm::Instruction mutate_sf(const arm::Instruction& instruction);
        arm::Instruction mutate_rm(const arm::Instruction& instruction);
        arm::Instruction mutate_rn(const arm::Instruction& instruction);
        arm::Instruction mutate_rd(const arm::Instruction& instruction);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
        std::discrete_distribution<> discrete;
    public:
        MutateDataProcessingReg2Source(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingReg2Source(std::mt19937& generator): MutateDataProcessingReg2Source(generator, {}) {}
        arm::Instruction mutate(const arm::Instruction &instruction) override;

    };

}


#endif //MEGUMIN_MUTATEDATAPROCESSINGREG_H
