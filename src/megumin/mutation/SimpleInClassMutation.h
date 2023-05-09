//
// Created by 58413 on 2023/4/20.
//

#ifndef MEGUMIN_SIMPLEINCLASSMUTATION_H
#define MEGUMIN_SIMPLEINCLASSMUTATION_H

#include <random>
#include <memory>
#include <InstructionDispatch.h>
#include "Mutation.h"

namespace megumin {
    class SimpleInClassMutation: public Mutation {
    private:
        arm::Instruction mutate_data_processing_imm(const arm::Instruction& instruction);
        arm::Instruction mutate_data_processing_reg(const arm::Instruction& instruction);

        std::unique_ptr<Mutation> mutate_add_sub_imm;
        std::unique_ptr<Mutation> mutate_logical_imm;
        std::unique_ptr<Mutation> mutate_move_wide_imm;
        std::unique_ptr<Mutation> mutate_bitfield;
        std::unique_ptr<Mutation> mutate_extract;

        std::unique_ptr<Mutation> mutate_source2;
        std::unique_ptr<Mutation> mutate_source1;
    public:
        explicit SimpleInClassMutation(std::mt19937& generator);

        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };
}



#endif //MEGUMIN_SIMPLEINCLASSMUTATION_H
