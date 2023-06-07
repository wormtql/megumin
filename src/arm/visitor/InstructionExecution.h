//
// Created by 58413 on 2023/6/2.
//

#ifndef MEGUMIN_INSTRUCTIONEXECUTION_H
#define MEGUMIN_INSTRUCTIONEXECUTION_H

#include "InstructionVisitor.h"
#include "MachineState.h"

namespace arm {
    class InstructionExecution: public arm::InstructionVisitor {
    private:
        MachineState& state;
    public:
        explicit InstructionExecution(MachineState& state);

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

        void visit_fp_simd_dp_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const Instruction &instruction) override;
    };
}

#endif //MEGUMIN_INSTRUCTIONEXECUTION_H
