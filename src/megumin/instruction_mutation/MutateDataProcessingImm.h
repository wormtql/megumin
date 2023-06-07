//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_MUTATEDATAPROCESSINGIMM_H
#define MEGUMIN_MUTATEDATAPROCESSINGIMM_H

#include <random>
#include <map>
#include <string>
#include <vector>

#include "InstructionMutation.h"
#include <Instruction.h>
#include <Program.h>

using std::string;

namespace megumin {
    // mutate add/sub imm
    class MutateDataProcessingImmAddSub: public InstructionMutation {
    public:
        struct Prob {
            int w_s = 1;
            int w_width = 1;
            int w_operator = 1;
            int w_rd = 5;
            int w_rn = 5;
            int w_imm12 = 10;
            int w_sh = 1;
        };
    public:
        explicit MutateDataProcessingImmAddSub(Prob probs);
        MutateDataProcessingImmAddSub(): MutateDataProcessingImmAddSub(Prob{}) {}
    };

    // mutate logical imm
    class MutateDataProcessingImmLogical: public InstructionMutation {
    public:
        struct Prob {
            int w_opc = 2;
            int w_n = 1;
            int w_sf = 1;
            int w_immr = 6;
            int w_imms = 6;
            int w_rn = 2;
            int w_rd = 2;
        };

        static arm::Instruction mutate_n(const arm::Program& program, int index);
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
        static arm::Instruction mutate_imms(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingImmLogical(Prob prob);
        MutateDataProcessingImmLogical(): MutateDataProcessingImmLogical(Prob{}) {}
    };

    // mutate move wide imm
    class MutateDataProcessingImmMoveWide: public InstructionMutation {
    public:
        struct Prob {
            int w_sf = 1;
            int w_opc = 3;
            int w_hw = 4;
            int w_imm16 = 16;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
        static arm::Instruction mutate_opc(const arm::Program& program, int index);
        static arm::Instruction mutate_hw(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingImmMoveWide(Prob prob);
        MutateDataProcessingImmMoveWide(): MutateDataProcessingImmMoveWide(Prob{}) {}
    };

    // mutate bitfield
    class MutateDataProcessingBitfield: public InstructionMutation {
    public:
        struct Prob {
            int w_sf_and_n = 0;
            int w_opc = 2;
            int w_immr = 6;
            int w_imms = 6;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_sf_and_n(const arm::Program& program, int index);
        static arm::Instruction mutate_opc(const arm::Program& program, int index);
        static arm::Instruction mutate_immr(const arm::Program& program, int index);
        static arm::Instruction mutate_imms(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingBitfield(Prob prob);
        MutateDataProcessingBitfield(): MutateDataProcessingBitfield(Prob{}) {}
    };

    // mutate extract
    class MutateDataProcessingExtract: public InstructionMutation {
    public:
        struct Prob {
            int w_sf = 1;
            int w_rm = 5;
            int w_imms = 6;
            int w_rn = 5;
            int w_rd = 5;
        };
    private:
        static arm::Instruction mutate_sf(const arm::Program& program, int index);
        static arm::Instruction mutate_imms(const arm::Program& program, int index);
    public:
        explicit MutateDataProcessingExtract(Prob prob);
        MutateDataProcessingExtract(): MutateDataProcessingExtract(Prob{}) {}
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGIMM_H
