//
// Created by 58413 on 2023/4/20.
//

#ifndef MEGUMIN_SIMPLEINCLASSMUTATION_H
#define MEGUMIN_SIMPLEINCLASSMUTATION_H

#include <random>

#include <InstructionDispatch.h>
#include "Mutation.h"

namespace megumin {
    class SimpleInClassMutation: public Mutation {
    private:
        arm::Instruction mutate_data_processing_imm(const arm::Instruction& instruction);

        Mutation* mutate_add_sub_imm;
        Mutation* mutate_logical_imm;
        Mutation* mutate_move_wide_imm;
    public:
        explicit SimpleInClassMutation(std::mt19937& generator);
        ~SimpleInClassMutation() override;

        arm::Instruction mutate(const arm::Instruction &instruction) override;
    };
}



#endif //MEGUMIN_SIMPLEINCLASSMUTATION_H
