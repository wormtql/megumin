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
        static arm::Instruction mutate_opcode(const arm::Program& program, arm::Program::ProgramPosition position);
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
        static arm::Instruction mutate_opcode(const arm::Program& program, arm::Program::ProgramPosition position);
    public:
        explicit MutateFPDataProcessing2(Prob prob);
        MutateFPDataProcessing2(): MutateFPDataProcessing2(Prob{}) {}
    };

    class MutateFPDataProcessing3: public InstructionMutation {
    public:
        struct Prob {
            int w_ptype = 1;
            int w_op = 1;
            int w_rm = 5;
            int w_ra = 5;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_opcode(const arm::Program& program, arm::Program::ProgramPosition position);
    public:
        explicit MutateFPDataProcessing3(Prob prob);
        MutateFPDataProcessing3(): MutateFPDataProcessing3(Prob{}) {}
    };

    class MutateFPImm: public InstructionMutation {
    public:
        struct Prob {
            int w_ptype = 1;
            int w_imm8 = 8;
            int w_rd = 5;
        };

        explicit MutateFPImm(Prob prob);
        MutateFPImm(): MutateFPImm(Prob{}) {}
    };

    class MutateFPCompare: InstructionMutation {
    public:
        struct Prob {
            int w_ptype = 1;
            int w_operand2 = 5;
            int w_rn = 5;
            int w_signal = 1;
        };
    private:
        static arm::Instruction mutate_operand2(const arm::Program& program, arm::Program::ProgramPosition position);
    public:
        explicit MutateFPCompare(Prob prob);
        MutateFPCompare(): MutateFPCompare(Prob{}) {}
    };
}



#endif //MEGUMIN_MUTATEFPDATAPROCESSING_H
