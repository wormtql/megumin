//
// Created by 58413 on 2023/4/17.
//

#include "InstructionMutation.h"

namespace megumin {
    std::uniform_int_distribution<> InstructionMutation::uniform_int;
    std::mt19937 InstructionMutation::generator{100};

    arm::Instruction InstructionMutation::LambdaMutateBit::operator()(const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        result.inverse_bit(mutate_index);
        return result;
    }

    arm::Instruction InstructionMutation::LambdaMutateRange::operator()(const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        int size = high - low;
        result.set_range(low, high, uniform_int(generator) % (1 << size));
        return result;
    }

    InstructionMutation::InstructionMutation(std::initializer_list<MutationFuncPair> func_pairs) {
        for (const auto& item: func_pairs) {
            for (int i = 0; i < item.second; i++) {
                mutation_functions.push_back(item.first);
            }
        }
    }

    arm::Instruction InstructionMutation::mutate_rn(const arm::Program& program, int index) {
        auto result = program.get_instruction_const(index);

        const auto& def_ins = program.get_def_in(index);
        result.set_range(5, 10, def_ins.random_gp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rd(const arm::Program& program, int index) {
        auto result = program.get_instruction_const(index);
        result.set_range(0, 5, uniform_int(generator) % (1 << 5));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rm(const arm::Program& program, int index) {
        auto result = program.get_instruction_const(index);

        const auto& def_ins = program.get_def_in(index);
        result.set_range(16, 21, def_ins.random_gp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_bit(int mutate_index, const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        result.inverse_bit(mutate_index);
        return result;
    }

    arm::Instruction InstructionMutation::mutate_range(int low, int high,
                                                       const arm::Program &program, int index) {
        auto result = program.get_instruction_const(index);
        int size = high - low;
        result.set_range(low, high, uniform_int(generator) % (1 << size));
        return result;
    }

    arm::Instruction InstructionMutation::mutate(const arm::Program &program, int index) {
        int i = uniform_int(generator) % mutation_functions.size();
        return mutation_functions[i](program, index);
    }
}
