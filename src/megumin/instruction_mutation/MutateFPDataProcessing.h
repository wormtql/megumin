//
// Created by 58413 on 2023/5/11.
//

#ifndef MEGUMIN_MUTATEFPDATAPROCESSING_H
#define MEGUMIN_MUTATEFPDATAPROCESSING_H

#include <random>
#include "instruction_mutation//InstructionMutation.h"

namespace megumin {
    class MutateFPDataProcessing1: public InstructionMutation {
    public:
        struct Prob {
            double w_ptype = 1.0;
            double w_opcode = 5.0;
            double w_rn = 5.0;
            double w_rd = 5.0;
        };
    private:
        arm::Instruction mutate_ptype(const arm::Instruction& instruction);
        arm::Instruction mutate_opcode(const arm::Instruction& instruction);
        arm::Instruction mutate_rn(const arm::Instruction& instruction);
        arm::Instruction mutate_rd(const arm::Instruction& instruction);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
        std::discrete_distribution<> discrete;
    public:
        MutateFPDataProcessing1(std::mt19937& generator, Prob prob);
        explicit MutateFPDataProcessing1(std::mt19937& generator): MutateFPDataProcessing1(generator, {}) {}
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };


    class MutateFPDataProcessing2: public InstructionMutation {
    public:
        struct Prob {
            double w_ptype = 1.0;
            double w_rm = 5.0;
            double w_opcode = 4.0;
            double w_rn = 5.0;
            double w_rd = 5.0;
        };
    private:
        arm::Instruction mutate_ptype(const arm::Instruction& instruction);
        arm::Instruction mutate_rm(const arm::Instruction& instruction);
        arm::Instruction mutate_opcode(const arm::Instruction& instruction);
        arm::Instruction mutate_rn(const arm::Instruction& instruction);
        arm::Instruction mutate_rd(const arm::Instruction& instruction);

        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
        std::discrete_distribution<> discrete;
    public:
        MutateFPDataProcessing2(std::mt19937& generator, Prob prob);
        explicit MutateFPDataProcessing2(std::mt19937& generator): MutateFPDataProcessing2(generator, {}) {}
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };
}



#endif //MEGUMIN_MUTATEFPDATAPROCESSING_H
