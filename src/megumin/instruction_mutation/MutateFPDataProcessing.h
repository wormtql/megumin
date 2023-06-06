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
            int w_ptype = 1;
            int w_opcode = 5;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_ptype(const arm::Program& program, int index);
        static arm::Instruction mutate_opcode(const arm::Program& program, int index);
    public:
        explicit MutateFPDataProcessing1(Prob prob);
        MutateFPDataProcessing1(): MutateFPDataProcessing1(Prob{}) {}
    };


    class MutateFPDataProcessing2: public InstructionMutation {
    public:
        struct Prob {
            int w_ptype = 1;
            int w_rm = 5;
            int w_opcode = 4;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_ptype(const arm::Program& program, int index);
        static arm::Instruction mutate_opcode(const arm::Program& program, int index);
    public:
        explicit MutateFPDataProcessing2(Prob prob);
        MutateFPDataProcessing2(): MutateFPDataProcessing2(Prob{}) {}
    };
}



#endif //MEGUMIN_MUTATEFPDATAPROCESSING_H
