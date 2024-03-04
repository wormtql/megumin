//
// Created by 58413 on 2023/6/2.
//

#ifndef MEGUMIN_INSTRUCTIONEXECUTION_H
#define MEGUMIN_INSTRUCTIONEXECUTION_H

#include "InstructionVisitor.h"
#include "MachineState.h"

#include <optional>

namespace arm {
    class InstructionExecution: public arm::InstructionVisitor {
    private:
        MachineState& state;
        /// none indicated the machine must execute sequentially
        /// while some(id) means the machine must goto the specified basic block
        std::optional<int> next_basic_block;
    public:
        explicit InstructionExecution(MachineState& state);

        std::optional<int> get_next_basic_block() {
            return next_basic_block;
        }

        void execute(const Instruction& instruction) { visit_instruction(instruction); }

        void visit_dp_imm_add_sub(const Instruction &instruction) override;

        void visit_dp_imm_logical(const Instruction &instruction) override;

        void visit_dp_imm_add_sub_with_tags(const Instruction &instruction) override;

        void visit_dp_imm_move_wide(const Instruction &instruction) override;

        void visit_dp_imm_bitfield(const Instruction &instruction) override;

        void visit_dp_imm_extract(const Instruction &instruction) override;

        void visit_dp_reg_1source(const Instruction &instruction) override;

        void visit_dp_reg_2source(const Instruction &instruction) override;

        void visit_dp_reg_logical_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_cond_select(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_with_carry(const Instruction &instruction) override;

        void visit_dp_reg_3source(const Instruction &instruction) override;

        void visit_fp_simd_dp_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const Instruction &instruction) override;

        void visit_fp_simd_dp_3source(const Instruction &instruction) override;

        void visit_fp_simd_imm(const Instruction &instruction) override;

        void visit_fp_compare(const Instruction &instruction) override;

        void visit_conditional_branch(const Instruction& instruction) override;

        void visit_unconditional_branch_immediate(const Instruction &instruction) override;

        void visit_unconditional_branch_register(const Instruction &instruction) override;

        void visit_compare_and_branch(const Instruction &instruction) override;

        void visit_test_and_branch(const Instruction &instruction) override;
    };
}

#endif //MEGUMIN_INSTRUCTIONEXECUTION_H
