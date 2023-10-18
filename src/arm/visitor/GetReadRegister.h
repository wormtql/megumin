//
// Created by 58413 on 2023/6/6.
//

#ifndef MEGUMIN_GETREADREGISTER_H
#define MEGUMIN_GETREADREGISTER_H

#include "InstructionVisitor.h"
#include "Instruction.h"

namespace arm {
    class GetReadRegister: public InstructionVisitor {
    private:
        void add_rn(const Instruction& instruction);
        void add_rm(const Instruction& instruction);
        void add_rn_fp(const Instruction& instruction);
        void add_rm_fp(const Instruction& instruction);
    public:
        // at most 3 registers
        Reg results[3];
        int size = 0;

        void visit_nop(const Instruction &instruction) override;

        void visit_dp_imm_add_sub(const Instruction &instruction) override;

        void visit_dp_imm_add_sub_with_tags(const Instruction &instruction) override;

        void visit_dp_imm_logical(const Instruction &instruction) override;

        void visit_dp_imm_move_wide(const Instruction &instruction) override;

        void visit_dp_imm_bitfield(const Instruction &instruction) override;

        void visit_dp_imm_extract(const Instruction &instruction) override;

        void visit_dp_reg_2source(const Instruction &instruction) override;

        void visit_dp_reg_1source(const Instruction &instruction) override;

        void visit_dp_reg_logical_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_cond_select(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_with_carry(const Instruction &instruction) override;

        void visit_fp_simd_dp_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const Instruction &instruction) override;

        void visit_fp_simd_imm(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_GETREADREGISTER_H
