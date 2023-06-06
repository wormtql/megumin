//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_INSTRUCTIONMUTATION_H
#define MEGUMIN_INSTRUCTIONMUTATION_H

#include <random>
#include <vector>
#include <functional>
#include <initializer_list>
#include <Instruction.h>
#include <Program.h>

namespace megumin {
    class InstructionMutation {
    private:
        std::vector<std::function<arm::Instruction(const arm::Program&, int)>> mutation_functions;
    protected:
        struct LambdaMutateBit {
            int mutate_index;
            arm::Instruction operator()(const arm::Program& program, int index);
        };

        struct LambdaMutateRange {
            int low;
            int high;
            arm::Instruction operator()(const arm::Program& program, int index);
        };

        static std::uniform_int_distribution<> uniform_int;
        static std::mt19937 generator;

        static arm::Instruction mutate_rn(const arm::Program& program, int index);
        static arm::Instruction mutate_rd(const arm::Program& program, int index);
        static arm::Instruction mutate_rm(const arm::Program& program, int index);
        static arm::Instruction mutate_bit(int mutate_index, const arm::Program& program, int index);
        static arm::Instruction mutate_range(int low, int high, const arm::Program& program, int index);

        static LambdaMutateBit make_mutate_bit(int mutate_index) {
            return LambdaMutateBit {
                .mutate_index=mutate_index
            };
        }

        static LambdaMutateRange make_mutate_range(int low, int high) {
            return LambdaMutateRange {
                .low=low,
                .high=high
            };
        }
    public:
        // func, weight
        using MutationFuncPair = std::pair<std::function<arm::Instruction(const arm::Program&, int)>, int>;

        InstructionMutation() = default;
        InstructionMutation(std::initializer_list<MutationFuncPair> func_pairs);
        virtual ~InstructionMutation() = default;

//        virtual arm::Instruction mutate(const arm::Instruction& instruction) = 0;
        virtual arm::Instruction mutate(const arm::Program& program, int index);
    };
}



#endif //MEGUMIN_INSTRUCTIONMUTATION_H
