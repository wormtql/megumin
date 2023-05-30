//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_RANDOMDATAPROCESSINGIMM_H
#define MEGUMIN_RANDOMDATAPROCESSINGIMM_H

#include <random>
#include <memory>
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

    class RandomMoveWideImm: public RandomInstruction {
    private:
        std::mt19937& generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomMoveWideImm(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction() override;
    };

    class RandomBitfield: public RandomInstruction {
    private:
        std::mt19937 &generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomBitfield(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction() override;
    };

    class RandomExtract: public RandomInstruction {
    private:
        std::mt19937 &generator;
        std::uniform_int_distribution<> uniform_int;
    public:
        explicit RandomExtract(std::mt19937& generator): generator(generator) {}

        arm::Instruction random_instruction() override;
    };

    class RandomDataProcessingImm: public RandomInstruction {
    public:
        struct Prob {
            double sub_add_imm = 1.0;
            double sub_add_imm_with_tags = 0.1;
            double logical = 1.0;
            double move_wide = 0.8;
//            double bitfield = 0.8;
            double bitfield = 1.0;
            double extract = 0.0;
        };
    private:
        std::mt19937& generator;
        std::discrete_distribution<> cat_prob;

//        std::vector<RandomInstruction*> dispatches;
        std::vector<std::unique_ptr<RandomInstruction>> dispatches;
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

            dispatches.push_back(std::make_unique<RandomAddSubImm>(generator));
            dispatches.push_back(std::make_unique<RandomAddSubImm>(generator));
            dispatches.push_back(std::make_unique<RandomDataProcessingImmLogical>(generator));
            dispatches.push_back(std::make_unique<RandomMoveWideImm>(generator));
            dispatches.push_back(std::make_unique<RandomBitfield>(generator));
            dispatches.push_back(std::make_unique<RandomExtract>(generator));
//            dispatches.push_back(std::make_unique<RandomAddSubImm>(generator));
            // todo
        }
        explicit RandomDataProcessingImm(std::mt19937& generator): RandomDataProcessingImm(generator, {}) {}

        arm::Instruction random_instruction() override;
    };
}



#endif //MEGUMIN_RANDOMDATAPROCESSINGIMM_H
