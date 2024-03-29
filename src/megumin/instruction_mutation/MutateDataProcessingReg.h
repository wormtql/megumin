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
            int w_opcode = 3;
            int w_sf = 1;
            int w_rm = 5;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_opcode(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingReg2Source(Prob prob);
        MutateDataProcessingReg2Source(): MutateDataProcessingReg2Source(Prob{}) {}
    };

    class MutateDataProcessingReg1Source: public InstructionMutation {
    public:
        struct Prob {
            int w_sf = 1;
            int w_opcode = 2;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
        static arm::Instruction mutate_opcode(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingReg1Source(Prob prob = Prob{.w_sf=1, .w_opcode=2, .w_rn=5, .w_rd=5});
    };

    class MutateDataProcessingRegLogical: public InstructionMutation {
    public:
        struct Prob {
#ifdef MEGUMIN_INST_64_ONLY
            int w_sf = 0;
#else
            int w_sf = 1;
#endif
            int w_opc = 1;
            int w_shift = 1;
            int w_N = 1;
            int w_rm = 5;
            int w_imm6 = 1;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_imm6(const arm::Program& program, int index);
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingRegLogical(Prob prob);
        explicit MutateDataProcessingRegLogical(): MutateDataProcessingRegLogical(Prob{}) {}
    };

    class MutateDataProcessingRegAddSubShiftedReg: public InstructionMutation {
    public:
        struct Prob {
#ifdef MEGUMIN_INST_64_ONLY
            int w_sf = 0;
#else
            int w_sf = 1;
#endif
            int w_op = 1;
            int w_S = 1;
            int w_shift = 3;
            int w_rm = 5;
            int w_imm6 = 6;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_imm6(const arm::Program& program, int index);
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
        static arm::Instruction mutate_shift(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingRegAddSubShiftedReg(Prob prob);
        explicit MutateDataProcessingRegAddSubShiftedReg(): MutateDataProcessingRegAddSubShiftedReg(Prob{}) {}
    };

    class MutateDataProcessingRegAddSubWithCarry: public InstructionMutation {
    public:
        struct Prob {
#ifdef MEGUMIN_INST_64_ONLY
            int w_sf = 0;
#else
            int w_sf = 1;
#endif
            int w_op = 1;
            int w_S = 1;
            int w_rm = 5;
            int w_rn = 5;
            int w_rd = 5;
        };
    public:
        explicit MutateDataProcessingRegAddSubWithCarry(Prob prob);
        MutateDataProcessingRegAddSubWithCarry(): MutateDataProcessingRegAddSubWithCarry(Prob{}) {}
    };

    class MutateDataProcessingRegCondSelect: public InstructionMutation {
    private:
        static arm::Instruction mutate_op(const arm::Program& program, int index);
        static arm::Instruction mutate_cond(const arm::Program& program, int index);
    public:
        struct Prob {
#ifdef MEGUMIN_INST_64_ONLY
            int w_sf = 0;
#else
            int w_sf = 1;
#endif
            int w_op = 3;
            int w_rm = 5;
            int w_cond = 4;
            int w_rn = 5;
            int w_rd = 5;
        };

        explicit MutateDataProcessingRegCondSelect(Prob prob);
        MutateDataProcessingRegCondSelect(): MutateDataProcessingRegCondSelect(Prob{}) {}
    };

    class MutateDataProcessingReg3Source: public InstructionMutation {
    private:
        static arm::Instruction mutate_op(const arm::Program& program, int index);
    public:
        struct Prob {
            int w_op = 3;
            int w_rm = 5;
            int w_ra = 5;
            int w_rn = 5;
            int w_rd = 5;
        };

        explicit MutateDataProcessingReg3Source(Prob prob);
        MutateDataProcessingReg3Source(): MutateDataProcessingReg3Source(Prob{}) {}
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGREG_H
