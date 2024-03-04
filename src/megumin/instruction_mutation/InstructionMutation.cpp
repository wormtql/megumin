//
// Created by 58413 on 2023/4/17.
//

#include "InstructionMutation.h"

namespace megumin {
    std::uniform_int_distribution<> InstructionMutation::uniform_int;
    std::mt19937 InstructionMutation::generator{100};

    arm::Instruction InstructionMutation::LambdaMutateBit::operator()(const arm::Program& program, arm::Program::ProgramPosition position) const {
        auto result = program.get_instruction_const(position);
        result.inverse_bit(mutate_index);
        return result;
    }

    arm::Instruction InstructionMutation::LambdaMutateRange::operator()(const arm::Program& program, arm::Program::ProgramPosition position) const {
        auto result = program.get_instruction_const(position);
        int size = high - low;
        if (bitwise) {
            result.inverse_bit(low + uniform_int(generator) % size);
            return result;
        } else {
            result.set_range(low, high, uniform_int(generator) % (1 << size));
            return result;
        }
    }

    InstructionMutation::InstructionMutation(std::initializer_list<MutationFuncPair> func_pairs) {
        for (const auto& item: func_pairs) {
            for (int i = 0; i < item.second; i++) {
                mutation_functions.push_back(item.first);
            }
        }
    }

    arm::Instruction InstructionMutation::mutate_rn(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(5, 10, def_ins.random_gp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rd(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);
        result.set_range(0, 5, uniform_int(generator) % 32);
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rm(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(16, 21, def_ins.random_gp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_ra(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(10, 15, def_ins.random_gp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rn_fp(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(5, 10, def_ins.random_fp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rm_fp(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(16, 21, def_ins.random_fp(generator));
        return result;
    }

    arm::Instruction InstructionMutation::mutate_rd_fp(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);
        result.set_range(0, 5, uniform_int(generator) % 32);
        return result;
    }

    arm::Instruction InstructionMutation::mutate(const arm::Program& program, arm::Program::ProgramPosition position) {
        int i = uniform_int(generator) % mutation_functions.size();
        return mutation_functions[i](program, position);
    }

    arm::Instruction InstructionMutation::mutate_fp_ptype(const arm::Program& program, arm::Program::ProgramPosition position) {
        const arm::Instruction& instruction = program.get_instruction_const(position);
        auto result = instruction;
        // todo half precision
        result.set_range(22, 24, uniform_int(generator) % 2);
        return result;
    }

    arm::Instruction InstructionMutation::mutate_ra_fp(const arm::Program& program, arm::Program::ProgramPosition position) {
        auto result = program.get_instruction_const(position);

        const auto& def_ins = program.get_def_in(position);
        result.set_range(10, 15, def_ins.random_fp(generator));
        return result;
    }
}
