//
// Created by 58413 on 2023/4/18.
//

#include "SimpleProgramMutation.h"

namespace megumin {
    arm::Program SimpleProgramMutation::mutate(const arm::Program &program) const {
        arm::Program result{program};
        int index = dist(generator);
        if (index == 0) {
            // delete
            int delete_index = uniform_int_dist(generator) % program.get_size();
            result.set_instruction_nop(delete_index);
        } else if (index == 1) {
            // change
            int change_index = uniform_int_dist(generator) % program.get_size();
            arm::Instruction new_inst = random_instruction_dispatch->random_instruction();
            result.set_instruction(change_index, new_inst);
        } else if (index == 2) {
            // swap
            int i1 = uniform_int_dist(generator) % program.get_size();
            int i2 = uniform_int_dist(generator) % program.get_size();
            result.swap_instructions(i1, i2);
        } else if (index == 3) {
            int mutate_index = uniform_int_dist(generator) % program.get_size();
            arm::Instruction new_inst = instruction_mutation->mutate(program.get_instruction_const(mutate_index));
            result.set_instruction(mutate_index, new_inst);
        }

        return result;
    }

    SimpleProgramMutation::SimpleProgramMutation(Prob prob, std::mt19937& generator, RandomInstruction* random_instruction, Mutation* instruction_mutation)
        : prob(prob), generator(generator), uniform_int_dist(),
          random_instruction_dispatch(random_instruction),
          instruction_mutation(instruction_mutation) {
        dist = std::discrete_distribution<>{{
            prob.delete_instruction,
            prob.change_instruction,
            prob.swap_instruction,
            prob.mutate_internal,
        }};
    }
}
