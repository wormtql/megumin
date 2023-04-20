//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_MUTATEDATAPROCESSINGIMM_H
#define MEGUMIN_MUTATEDATAPROCESSINGIMM_H

#include <random>
#include <map>
#include <string>
#include <vector>

#include "Mutation.h"
#include <Instruction.h>

using std::string;

namespace megumin {
    // mutate add/sub imm
    class MutateDataProcessingImmAddSubS: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSubWidth: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSubOperator: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSubRd: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSubRn: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSubImm12: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    // mutate sh flag
    class MutateDataProcessingImmAddSubSh: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmAddSub: public Mutation {
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
        std::vector<Mutation*> mutations;
    public:
        MutateDataProcessingImmAddSub(std::mt19937& generator, Prob probs);
        explicit MutateDataProcessingImmAddSub(std::mt19937& generator): MutateDataProcessingImmAddSub(generator, {}) {}
        ~MutateDataProcessingImmAddSub() override;

        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    // mutate logical imm
    class MutateDataProcessingImmLogicalOpc: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalN: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalSf: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalImmr: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalImms: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalRn: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogicalRd: public Mutation {
    public:
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    class MutateDataProcessingImmLogical: public Mutation {
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
    private:
        std::mt19937& generator;
        std::discrete_distribution<> dist;
        std::vector<Mutation*> mutations;
    public:
        MutateDataProcessingImmLogical(std::mt19937& generator, Prob prob);
        ~MutateDataProcessingImmLogical() override;
        explicit MutateDataProcessingImmLogical(std::mt19937& generator): MutateDataProcessingImmLogical(generator, {}) {}
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };

    // mutate move wide imm
    class MutateDataProcessingImmMoveWide: public Mutation {
    public:
        struct Prob {
            double w_sf = 1.0;
            double w_opc = 1.0;
            double w_hw = 1.0;
            double w_imm16 = 10.0;
            double w_rd = 2.0;
        };
    private:
        arm::Instruction mutate_sf(const arm::Instruction& instruction);
        arm::Instruction mutate_opc(const arm::Instruction& instruction);
        arm::Instruction mutate_hw(const arm::Instruction& instruction);
        arm::Instruction mutate_imm16(const arm::Instruction& instruction);
        arm::Instruction mutate_rd(const arm::Instruction& instruction);

        std::mt19937& generator;
        std::discrete_distribution<> discrete;
        std::uniform_int_distribution<> uniform_int;
    public:
        MutateDataProcessingImmMoveWide(std::mt19937& generator, Prob prob);
        explicit MutateDataProcessingImmMoveWide(std::mt19937& generator): MutateDataProcessingImmMoveWide(generator, {}) {}
        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGIMM_H
