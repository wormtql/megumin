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
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGIMM_H
