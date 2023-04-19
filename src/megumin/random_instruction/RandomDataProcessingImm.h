//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_RANDOMDATAPROCESSINGIMM_H
#define MEGUMIN_RANDOMDATAPROCESSINGIMM_H

#include <random>
#include <vector>
#include "random_instruction/RandomInstruction.h"

namespace megumin {
    class RandomAddSubImm: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int_dist;
    public:
        explicit RandomAddSubImm(std::mt19937& generator);
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingImmLogical: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int_distribution;
    public:
        explicit RandomDataProcessingImmLogical(std::mt19937& generator): generator(generator) {}
        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingImm: public RandomInstruction {
    public:
        struct Prob {
            double sub_add_imm = 1.0;
            double sub_add_imm_with_tags = 0.1;
            double logical = 1.0;
            double move_wide = 0.8;
            double bitfield = 0.8;
            double extract = 1.0;
        };
    private:
        std::mt19937& generator;
        std::discrete_distribution<> cat_prob;

        std::vector<RandomInstruction*> dispatches;
    public:
        explicit RandomDataProcessingImm(std::mt19937& generator, Prob prob): generator(generator) {
            cat_prob = {{
                prob.sub_add_imm,
                prob.sub_add_imm_with_tags,
                prob.logical,
                prob.move_wide,
                prob.bitfield,
                prob.extract,
            }};

            dispatches.push_back(new RandomAddSubImm(generator));
            dispatches.push_back(new RandomAddSubImm(generator));
            dispatches.push_back(new RandomAddSubImm(generator));
            dispatches.push_back(new RandomAddSubImm(generator));
            dispatches.push_back(new RandomAddSubImm(generator));
            dispatches.push_back(new RandomAddSubImm(generator));
            // todo
        }
        explicit RandomDataProcessingImm(std::mt19937& generator): RandomDataProcessingImm(generator, {}) {}
        ~RandomDataProcessingImm() override {
            for (RandomInstruction* r : dispatches) {
                delete r;
            }
        }

        arm::Instruction random_instruction() override;
    };
}



#endif //MEGUMIN_RANDOMDATAPROCESSINGIMM_H
