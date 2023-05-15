//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_SIMPLEPROGRAMMUTATION_H
#define MEGUMIN_SIMPLEPROGRAMMUTATION_H

#include <map>
#include <string>
#include <random>

#include "mutation/ProgramMutation.h"
#include "mutation/Mutation.h"
#include "random_instruction/RandomInstruction.h"

using std::string;
using std::map;

namespace megumin {
    class SimpleProgramMutation: public ProgramMutation {
    public:
        struct Prob {
            double delete_instruction = 1.0;
            double change_instruction = 1.0;
            double swap_instruction = 1.0;
            double mutate_internal = 1.0;
        };
    private:
        SimpleProgramMutation::Prob prob;
        std::discrete_distribution<> dist;
        std::uniform_int_distribution<> uniform_int_dist;
        std::mt19937& generator;

        RandomInstruction* random_instruction_dispatch = nullptr;
        Mutation* instruction_mutation = nullptr;
    public:
        explicit SimpleProgramMutation(Prob prob, std::mt19937& generator, RandomInstruction* random_instruction, Mutation* instruction_mutation);
        explicit SimpleProgramMutation(std::mt19937& generator, RandomInstruction* random_instruction, Mutation* instruction_mutation)
            : SimpleProgramMutation({}, generator, random_instruction, instruction_mutation) {}

        void set_random_instruction_dispatch(RandomInstruction* random_instruction) {
            this->random_instruction_dispatch = random_instruction;
        }

        void set_instruction_mutation(Mutation* mutation) {
            this->instruction_mutation = mutation;
        }

        [[nodiscard]] arm::Program mutate(const arm::Program &program) override;
    };
}



#endif //MEGUMIN_SIMPLEPROGRAMMUTATION_H
