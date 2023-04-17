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
    private:
        double w_s;
        double w_width;
        double w_operator;
        double w_rd;
        double w_rn;
        double w_imm12;
        double w_sh;

        std::mt19937& generator;
        std::discrete_distribution<> dist;
        std::vector<Mutation*> mutations;
    public:
        MutateDataProcessingImmAddSub(std::mt19937& generator, std::map<string, double> weights);

        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };
}


#endif //MEGUMIN_MUTATEDATAPROCESSINGIMM_H
