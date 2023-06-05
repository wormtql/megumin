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
            double w_s = 1.0;
            double w_width = 1.0;
            double w_operator = 1.0;
            double w_rd = 5.0;
            double w_rn = 5.0;
            double w_imm12 = 10.0;
            double w_sh = 1.0;
        };
    private:
        std::mt19937& generator;
        std::discrete_distribution<> dist;
        std::uniform_int_distribution<> uniform_int;

        arm::Instruction mutate_s(const arm::Program& program, int index);
        arm::Instruction mutate_width(const arm::Program& program, int index);
        arm::Instruction mutate_operator(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_imm12(const arm::Program& program, int index);
        arm::Instruction mutate_sh(const arm::Program& program, int index);
    public:
        MutateDataProcessingImmAddSub(std::mt19937& generator, Prob probs);
        explicit MutateDataProcessingImmAddSub(std::mt19937& generator): MutateDataProcessingImmAddSub(generator, {}) {}

        arm::Instruction mutate(const arm::Program& program, int index) override;
    };

    // mutate logical imm
    class MutateDataProcessingImmLogical: public InstructionMutation {
    public:
        struct Prob {
            double w_opc = 1.0;
            double w_n = 0.5;
            double w_sf = 0.5;
            double w_immr = 3.0;
            double w_imms = 3.0;
            double w_rn = 1.0;
            double w_rd = 1.0;
        };

        arm::Instruction mutate_opc(const arm::Program& program, int index);
        arm::Instruction mutate_n(const arm::Program& program, int index);
        arm::Instruction mutate_sf(const arm::Program& program, int index);
        arm::Instruction mutate_immr(const arm::Program& program, int index);
        arm::Instruction mutate_imms(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);
    private:
        std::mt19937& generator;
        std::discrete_distribution<> dist;
        std::uniform_int_distribution<> uniform_int;
    public:
        MutateDataProcessingImmLogical(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingImmLogical(std::mt19937& generator): MutateDataProcessingImmLogical(generator, {}) {}
        arm::Instruction mutate(const arm::Program& program, int index) override;
    };

    // mutate move wide imm
    class MutateDataProcessingImmMoveWide: public InstructionMutation {
    public:
        struct Prob {
            double w_sf = 1.0;
            double w_opc = 1.0;
            double w_hw = 1.0;
            double w_imm16 = 10.0;
            double w_rd = 2.0;
        };
    private:
        arm::Instruction mutate_sf(const arm::Program& program, int index);
        arm::Instruction mutate_opc(const arm::Program& program, int index);
        arm::Instruction mutate_hw(const arm::Program& program, int index);
        arm::Instruction mutate_imm16(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);

        std::mt19937& generator;
        std::discrete_distribution<> discrete;
        std::uniform_int_distribution<> uniform_int;
    public:
        MutateDataProcessingImmMoveWide(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingImmMoveWide(std::mt19937& generator): MutateDataProcessingImmMoveWide(generator, {}) {}
        arm::Instruction mutate(const arm::Program& program, int index) override;
    };

    // mutate bitfield
    class MutateDataProcessingBitfield: public InstructionMutation {
    public:
        struct Prob {
            double w_sf_and_n = 0.2;
            double w_opc = 2.0;
            double w_immr = 6.0;
            double w_imms = 6.0;
            double w_rn = 5.0;
            double w_rd = 5.0;
        };
    private:
        arm::Instruction mutate_sf_and_n(const arm::Program& program, int index);
        arm::Instruction mutate_opc(const arm::Program& program, int index);
        arm::Instruction mutate_immr(const arm::Program& program, int index);
        arm::Instruction mutate_imms(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);

        std::mt19937 &generator;
        std::discrete_distribution<> discrete;
        std::uniform_int_distribution<> uniform_int;
    public:
        MutateDataProcessingBitfield(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingBitfield(std::mt19937& generator): MutateDataProcessingBitfield(generator, {}) {}

        arm::Instruction mutate(const arm::Program& program, int index) override;
    };

    // mutate extract
    class MutateDataProcessingExtract: public InstructionMutation {
    public:
        struct Prob {
            double w_sf = 1.0;
            double w_rm = 5.0;
            double w_imms = 6.0;
            double w_rn = 5.0;
            double w_rd = 5.0;
        };
    private:
        arm::Instruction mutate_sf(const arm::Program& program, int index);
        arm::Instruction mutate_rm(const arm::Program& program, int index);
        arm::Instruction mutate_imms(const arm::Program& program, int index);
        arm::Instruction mutate_rn(const arm::Program& program, int index);
        arm::Instruction mutate_rd(const arm::Program& program, int index);

        std::mt19937 &generator;
        std::discrete_distribution<> discrete;
        std::uniform_int_distribution<> uniform_int;
    public:
        MutateDataProcessingExtract(std::mt19937 &generator, Prob prob);
        explicit MutateDataProcessingExtract(std::mt19937& generator): MutateDataProcessingExtract(generator, {}) {}

        arm::Instruction mutate(const arm::Program& program, int index) override;
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGIMM_H
