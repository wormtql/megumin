//
// Created by 58413 on 2023/6/27.
//

#ifndef MEGUMIN_INSTRUCTIONEXECUTIONS_H
#define MEGUMIN_INSTRUCTIONEXECUTIONS_H

#include "MachineStateS.h"
#include "InstructionVisitor.h"

namespace arm {
    class InstructionExecutionS: public InstructionVisitor {
    private:
        MachineStateS& state;
    public:
        explicit InstructionExecutionS(MachineStateS& state): state(state) {}

        void visit_dp_imm_add_sub(const Instruction &instruction) override;

        void visit_dp_imm_logical(const Instruction &instruction) override;

        void visit_dp_imm_move_wide(const Instruction &instruction) override;

        void visit_dp_imm_bitfield(const Instruction &instruction) override;

        void visit_dp_imm_extract(const Instruction &instruction) override;

        void visit_dp_reg_1source(const Instruction &instruction) override;

        void visit_dp_reg_2source(const Instruction &instruction) override;

        void visit_dp_reg_logical_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_with_carry(const Instruction &instruction) override;

        void visit_dp_reg_cond_select(const Instruction &instruction) override;

        void visit_dp_reg_3source(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_INSTRUCTIONEXECUTIONS_H
