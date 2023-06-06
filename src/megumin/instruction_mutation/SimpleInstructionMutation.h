//
// Created by 58413 on 2023/6/4.
//

#ifndef MEGUMIN_SIMPLEINSTRUCTIONMUTATION_H
#define MEGUMIN_SIMPLEINSTRUCTIONMUTATION_H

#include <memory>
#include <random>

#include "InstructionVisitor.h"
#include "Instruction.h"
#include "MutateDataProcessingImm.h"
#include "MutateDataProcessingReg.h"
#include "MutateFPDataProcessing.h"

namespace megumin {
    class SimpleInstructionMutation: public arm::InstructionVisitor {
    private:
        MutateDataProcessingImmAddSub mutate_dp_imm_add_sub;
        MutateDataProcessingImmLogical mutate_dp_imm_logical;
        MutateDataProcessingImmMoveWide mutate_dp_imm_move_wide;
        MutateDataProcessingBitfield mutate_dp_imm_bitfield;
        MutateDataProcessingExtract mutate_dp_imm_extract;

        MutateDataProcessingReg2Source mutate_dp_reg_2source;
        MutateDataProcessingReg1Source mutate_dp_reg_1source;

        MutateFPDataProcessing1 mutate_fp_simd_dp_1source;
        MutateFPDataProcessing2 mutate_fp_simd_dp_2source;
    public:
        arm::Instruction result;
        const arm::Program* program = nullptr;
        int index = 0;

        explicit SimpleInstructionMutation(std::mt19937& generator);

        void visit_nop(const arm::Instruction &instruction) override;

        void visit_dp_imm_add_sub(const arm::Instruction &instruction) override;

        void visit_dp_imm_add_sub_with_tags(const arm::Instruction &instruction) override;

        void visit_dp_imm_logical(const arm::Instruction &instruction) override;

        void visit_dp_imm_move_wide(const arm::Instruction &instruction) override;

        void visit_dp_imm_bitfield(const arm::Instruction &instruction) override;

        void visit_dp_imm_extract(const arm::Instruction &instruction) override;

        void visit_dp_reg_2source(const arm::Instruction &instruction) override;

        void visit_dp_reg_1source(const arm::Instruction &instruction) override;

        void visit_fp_simd_dp_1source(const arm::Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const arm::Instruction &instruction) override;
    };
}




#endif //MEGUMIN_SIMPLEINSTRUCTIONMUTATION_H